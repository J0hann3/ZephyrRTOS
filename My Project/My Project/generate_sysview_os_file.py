#
# Modify the following variables with the right value
#

# If path to the Os file description needs permission, the script must be executed with admin permission
path_os_file_description = "C:\Program Files\SEGGER\SystemView\Description\\"      # Path to the SystemView description folder e.g. C:\Program Files\SEGGER\SystemView\Description\
os_name = "NoOS_NxgCourt"           # Name of the OS defined in _cbSendSystemDesc(){SEGGER_SYSVIEW_SendSysDesc("O=NoOS_NxgCourt");}
path_header_file = "My Project\My Project\SystemView"       # Path to create the header file with the definitions of all the events
list_json_file = ["My Project\My Project\SystemView.json", "My Project\My Project\\test.json"] # List of all the json file to parse to create the Os description and header file
#
# End of variable definition
#

import json
import os
import sys

class color:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def print_fail(message):
    print(f'{color.FAIL}Error: {message}{color.ENDC}')

def print_warning(message):
    print(f'{color.WARNING}Warning: {message}{color.ENDC}')

# -------------------- NamedType Class --------------------

class NamedType:
    named_types = []
    
    def __init__(self, type_name, fields):
        if not type_name:
            raise ValueError(f'Missing or empty "TypeName"') 
        if not fields:
            raise ValueError(f'Missing or empty "Fields"') 
        for nametype in NamedType.named_types:
            if type_name == nametype.type_name:
                raise ValueError(f'TypeName: "{type_name}" is already defined')
        self.type_name = type_name
        self.fields = {}
        i = 0
        for field in fields:
            if not field.get('Key'):
                raise ValueError(f'Missing or empty "Key" for "{type_name}"')
            if not field.get('Value'):
                raise ValueError(f'Missing or empty "Value" for "{type_name}"')
            try:
                int(field.get('Key'))
            except:
                raise ValueError(f'"Key" value must but a number')
            if field.get('Key') in self.fields:
                print_warning(f'Fields with key: "{field.get("Key")}" already exist in "{type_name}", ignore the second definition')
            elif field.get('Value') in self.fields.values():
                print_warning(f'Fields with value: "{field.get("Value")}" already exist in "{type_name}", ignore the second definition')
            else:
                self.fields[field.get('Key')] = field.get('Value')
            i += 1
        if i == 0:
            raise ValueError(f'Missing "Fields" elements for {type_name}')
        NamedType.named_types.append(self)
    
    def __str__(self):
        str = 'NamedType ' + self.type_name + ' '
        for key, value in self.fields.items():
            str += key + '=' + value + ' '
        str += '\n'
        return str

    def header_define(self):
        line = ''
        for key, value in self.fields.items():
            line += f'#define TYPE_SYSVIEW_{self.type_name.upper()}_{value.upper()} {key.upper()}\n'
        return line
    
    def parse_namedType(dict_json_data):
        for data in dict_json_data.values():
            for value in data.get('NamedType', []):
                NamedType(value.get('TypeName'), value.get('Fields'))

# -------------------- Function Class --------------------

class Function:
    functions = []
    __index = 32        # index before this are reserved for systemview

    def __init__(self, function):
        if not function.get('FunctionName'):
            raise ValueError(f'Missing or empty "FunctionName"')
        if Function.__index > 511:
            raise Exception('To many Function, the maximum amount of function is reached')
        for f in Function.functions:
            if function.get('FunctionName') == f.function_name:
                print_warning(f'Function with name "{f.function_name}" already exist with id: {f.index}')
        self.index = Function.__index
        Function.__index += 1
        self.function_name = function.get('FunctionName')
        if function.get('ParameterDescription'):
            self.ParameterDescription = function.get('ParameterDescription')
        if function.get('ReturnValueDescription'):
            self.ReturnValueDescription = function.get('ReturnValueDescription')
        Function.functions.append(self)

    def __str__(self):
        str = f'{self.index} {self.function_name}'
        if hasattr(self, 'ParameterDescription'):
            str += ' ' + self.ParameterDescription
        if hasattr(self, 'ReturnValueDescription'):
            str += ' | ' + self.ReturnValueDescription
        str += '\n'
        return str
    
    def header_define(self):
        return f'#define ID_SYSVIEW_{self.function_name.upper()} {self.index}\n'
    
    def parse_function(dict_json_data):
        for data in dict_json_data.values():
            for function in data.get('Function', []):
                Function(function)

def open_json_file():
    dict_json_data = {}

    for filename in list_json_file:
        try:
            file = open(filename)
            data = json.load(file)
            file.close()
        except Exception as error:
            print_fail(f"Error while parsing json file: '{filename}'")
            print(error)
            sys.exit()
        if filename in dict_json_data:
            print_warning(f"Duplicat json file: {filename}, ignore the duplicat file")
        dict_json_data[filename] = data
    if len(dict_json_data) == 0:
        print_warning("No json file found")
    return dict_json_data

def write_os_file_description():
    """Create file Os description for SystemView with NamedType and API Functions"""
    filename = "SYSVIEW_" + os_name + ".txt"
    path = os.path.join(path_os_file_description, filename)
    file = open(path, 'w')
    file.write(f"# File generated by the script {__file__}.\n\
# Do not modify it.\n\
# Modify the json configuration if wanted, then regenerate this file with the script.\n\
#\n\
# File    : {filename}\n\
# Purpose : No OS description file for SEGGER SystemView.\n\
#           Please copy this file to your SystemView installation folder,\n\
#           e.g. C:\Program Files\SEGGER\SystemView\Description\.\n\n")
    
    file.write("""#
# Types for parameter formatters
#\n""")
    for named_type in NamedType.named_types:
        file.write(str(named_type))
    file.write('\n')

    file.write("""#
# API Functions
#\n""")
    for function in Function.functions:
        file.write(str(function))
    file.write('\n')

    file.write("""#
# End of file
#
""")
    file.close()
    print(f"{color.OKGREEN}{filename} is generated{color.ENDC}")

def write_header_file():
    """Create C header file for SystemView with define NamedType and define API Functions"""
    filename = "SEGGER_SYSVIEW_Conf.g.h"
    path = os.path.join(path_header_file, filename)
    file = open(path, 'w')
    file.write(f"/*\n\
* File generated by the script '{__file__}'.\n\
* Do not modify it.\n\
* Modify the json configuration if wanted, then regenerate this file with the script.\n\
*\n\
* File    : {filename}\n\
* Purpose : SEGGER SystemView configuration file.\n*/\n\n")
    file.write("""#ifndef SEGGER_SYSVIEW_CONF_G_H
#define SEGGER_SYSVIEW_CONF_G_H\n\n""")

    file.write("""//
// Define Sysview NamedType to record events. 
//\n""")
    for named_type in NamedType.named_types:
        file.write(f'\n// NamedType {named_type.type_name}\n')
        line = named_type.header_define()
        file.write(line)
    file.write('\n')

    file.write("""//
// Define Sysview function ID to record events.
//\n""")
    for function in Function.functions:
        line = function.header_define()
        file.write(line)

    file.write("""\n#endif  // SEGGER_SYSVIEW_CONF_G_H\n""")
    file.close()
    print(f"{color.OKGREEN}{filename} is generated{color.ENDC}")

# -------------------- Main execution --------------------

def main():
    print(f"{color.OKCYAN}Start SystemView file generation{color.ENDC}")
    try:
        dict_json_data = open_json_file()
        NamedType.parse_namedType(dict_json_data)
        Function.parse_function(dict_json_data)
        write_os_file_description()
        write_header_file()
    except Exception as error:
        print_fail(error)
        print(f"{color.FAIL}Fail to generate files{color.ENDC}")

    print(f"{color.OKCYAN}End SystemView file generation{color.ENDC}")

if __name__ == "__main__":
    main()
    