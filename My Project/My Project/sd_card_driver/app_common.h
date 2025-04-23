#ifndef APP_H
#define APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "configuration.h"
#include "system/fs/sys_fs.h"

/* Application access

   Summary :
     Application access enumeration

   Description :
     This enumeration defines the valid application access. These access
     determine the behavior of the application at various times.
*/
typedef enum
{
  USUAL_ACCESS,
  TEST_ACCESS,
  FORMAT_ACCESS
} APP_ACCESS;

/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
  /* Application's state machine's initial state. */
  /* The app mounts the disk */
  APP_START = 0,

  APP_MOUNT_DISK,

  /* The app unmounts the disk */
  APP_UNMOUNT_DISK,

  /* The app mounts the disk again */
  APP_MOUNT_DISK_AGAIN,

  /* Set the current drive */
  APP_SET_CURRENT_DRIVE,

  /* The app opens the file to read */
  APP_OPEN_FIRST_FILE,

  /* Create directory */
  APP_CREATE_DIRECTORY,

  /* The app opens the file to write */
  APP_OPEN_THE_FILE,

  /* The app write first line of csv */
  APP_WRITE_FIRST_LINE_TO_FILE,

  /* The app write measure to file */
  APP_WRITE_TO_FILE,

  /* The app write test file*/
  APP_WRITE_TEST_FILE,

  /* The app check test file*/
  APP_CHECK_TEST_FILE,

  /* The app closes the file*/
  APP_CLOSE_FILE,

  /* The app delete test file*/
  APP_DELETE_TEST_FILE,

  /* The app format the drive */
  APP_FORMAT_DRIVE,

  /* An app error has occurred */
  APP_ERROR,

  /* The app ends here when copy is complete */
  APP_IDLE

} APP_STATES;

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
  /* SYS_FS File handle for 1st file */
  SYS_FS_HANDLE fileHandle;

  /* Application's current state */
  APP_STATES state;

  int32_t nBytesRead;
} APP_DATA;

void sd_card_set_CS_pin_as_input(void);

void sd_card_set_CS_pin_as_lowpower(void);

void sd_card_set_CS_pin_as_output(void);

void sd_card_power_on(void);

void sd_card_power_off(void);

bool sd_card_is_card_detected();

/************************************************************************/
/* Functions defined in b_sw_config.c                                   */
/************************************************************************/

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize(void);

/*******************************************************************************
  Function:
    void APP_Tasks ( APP_ACCESS access )

  Summary:
    Application tasks function to access to SD card

  Description:
    This routine is the SD card acces tasks function.  It
    defines the SD card acces application's state machine and core logic.

    A call with USUAL_ACCESS will write to SD card all measures in stack.
    A call with TEST_ACCESS will write a test line then read and compare it.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    Enum APP_ACCESS access

  Returns:
    None.

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_Tasks(APP_ACCESS access);

APP_STATES get_sd_app_state();
void       set_sd_app_state(APP_STATES status);

uint16_t get_sd_nb_writes(void);
void     set_sd_nb_writes(uint16_t value);

void sd_card_go_to_sleep();
void sd_card_wake_from_sleep(void);

#endif /* APP_H */

/*******************************************************************************
 End of File
 */
