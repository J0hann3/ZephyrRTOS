import argparse
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
        str = f'#define ID_SYSVIEW_{self.function_name.upper()} {self.index}       //'
        if hasattr(self, 'ParameterDescription'):
            str += f'{self.ParameterDescription}'
        if hasattr(self, 'ReturnValueDescription'):
            str += f' | {self.ReturnValueDescription}'
        str += '\n'
        return str
    
    def parse_function(dict_json_data):
        for data in dict_json_data.values():
            for function in data.get('Function', []):
                Function(function)

def open_json_file(list_json_file):
    if list_json_file == None:
        raise Exception('Missing argument "--json-files" that need to be parse')
    dict_json_data = {}

    for filename in list_json_file:
        try:
            with open(filename) as file:
                data = json.load(file)
        except Exception as error:
            print_fail(f"Error while parsing json file: '{filename}'")
            print(error)
            sys.exit()
        if filename in dict_json_data:
            print_warning(f"Duplicate json file: {filename}, ignore the duplicate file")
        dict_json_data[filename] = data
    if len(dict_json_data) == 0:
        print_warning("No json file found")
    return dict_json_data

def write_os_file_description(path_os_file_description, os_name):
    """Create file Os description for SystemView with NamedType and API Functions"""

    filename = "SYSVIEW_" + os_name + ".txt"
    path = os.path.join(path_os_file_description, 'Description')
    path = os.path.join(path, filename)
    with open(path, 'w') as file:
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
    print(f"{color.OKGREEN}{filename} is generated{color.ENDC}")

def write_header_file(path_header_file):
    """Create C header file for SystemView with define NamedType and define API Functions"""

    if path_header_file == None:
        raise Exception('Missing argument "--path-header" to generate header file')
    filename = "SEGGER_SYSVIEW_Conf.g.h"
    path = os.path.join(path_header_file, filename)
    with open(path, 'w') as file:
        file.write(f"/*\n\
* File generated by the script '{__file__}'.\n\
* Do not modify it.\n\
* Modify the json configuration if wanted, then regenerate this file with the script.\n\
*\n\
* File    : {filename}\n\
* Purpose : SEGGER SystemView configuration file.\n\
* \n\
* In the file Config/SEGGER_SYSVIEW_Conf.h add the following:\n\
*   #include {filename}\n\
* \n\
* To record an event, use the following function:\n\
* #include <SEGGER_SYSVIEW.h>\n\
*   SEGGER_SYSVIEW_RecordXX(ID_SYSVIEW_FUNCTION, XX, XX) => Where XX represents  the number or type of arguments\n\
*  to pass to SystemView, according to its definition in the .json files.\n\
* Or:\n\
*   SEGGER_SYSVIEW_RecordEndCallXX(ID_SYSVIEW_FUNCTION, XX) => Should be called to specify the end\n\
*  of the event that was recorded.\n\
* \n\
* Pass NamedType value to SEGGER_SYSVIEW_RecordXX(ID, TYPE_SYSVIEW_xx) if the ID_SYSVIEW takes\n\
*  NamedType in arguments. \n\
*/\n\n")
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
    print(f"{color.OKGREEN}{filename} is generated{color.ENDC}")

def get_path_os_file_description(args_path_os):
    path_os_file_description = ''
    if not args_path_os:
        path_os_file_description = os.environ.get('SYSVIEW_PATH', None)
        if not path_os_file_description:
            raise Exception(f'Missing environment variable "SYSVIEW_PATH" to create OS file description')
    else:
        path_os_file_description = args_path_os
    return path_os_file_description

# -------------------- Main execution --------------------

def main(args):
    print(f"{color.OKCYAN}Start SystemView file generation{color.ENDC}")
    try:
        path_os_file_description = get_path_os_file_description(args.path_OS)
        print(f"{color.OKGREEN}SYSVIEW_PATH: '{path_os_file_description}'{color.ENDC}")
        
        dict_json_data = open_json_file(args.json_files)
        NamedType.parse_namedType(dict_json_data)
        Function.parse_function(dict_json_data)
        write_os_file_description(path_os_file_description, args.Os_name)
        write_header_file(args.path_header)
        print(f"{color.OKCYAN}End SystemView file generation{color.ENDC}")
    except Exception as error:
        print_fail(error)
        print(f"{color.FAIL}Fail to generate files{color.ENDC}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(prog="sysview", description=f"usage: example: python {__file__} nxgCourt --json-files sysview.json sysview1.json sysview2.json --path-header .\src\SystemView")
    # Mandatory Arguments
    parser.add_argument("-O", "--Os-name", required=True, help='Name of the OS defined in _cbSendSystemDesc(){SEGGER_SYSVIEW_SendSysDesc("O=OsName");}.')
    parser.add_argument("-j", "--json-files", nargs='+', required=True, help="List of all the json file to parse to create the Os description file and header file.")
    parser.add_argument("-pH", "--path-header", required=True, help="Path to create the header file with the definitions of all the events.")
    # Optional Arguments
    parser.add_argument("-pO", "--path-OS", help="Path to the SystemView folder e.g. 'C:\Program Files\SEGGER\SystemView\\', if not specify use SYSVIEW_PATH env variable.")
    
    args = parser.parse_args()
    main(args)
    