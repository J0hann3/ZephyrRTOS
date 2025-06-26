// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdio.h>
#include <string.h>

#include "tools.h"
#include "atmel_start_pins.h"
#include "configuration.h"
#include "driver_init.h"
#include "measures_logger.h"
#include "definitions.h"
#include "SEGGER_SYSVIEW.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

#define SDCARD_MOUNT_NAME    "/mnt/mydrive"
#define SDCARD_DEV_NAME      "/dev/mmcblka1"

#define APP_DATA_LEN         512

#define MOUNTUNMOUNT_MAX_TRY 12000

/*---------------------------------*/
/*---     LOCAL PROTOTYPES      ---*/
/*---------------------------------*/
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA        appData;

// *****************************************************************************
// *****************************************************************************
// Section: System "Tasks" Routine
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Tasks ( void )

  Remarks:
    See prototype in system/common/sys_module.h.
*/
bool SYS_Tasks(APP_ACCESS access)
{
  bool error = true;

  sd_card_wake_from_sleep();

  /* Reading card detect input */
  if (sd_card_is_card_detected() != true)
  {
	#ifdef DEBUG
	  printf("Card not detected\n");
  #endif
    SEGGER_SYSVIEW_RecordU32(ID_SYSVIEW_E_SD_CARD_DETECTED, 0);
    SEGGER_SYSVIEW_Warn("No card detected\n");
  }
  else
  {
    SEGGER_SYSVIEW_RecordU32(ID_SYSVIEW_E_SD_CARD_DETECTED, 1);

    /* try to write until success (APP_IDLE) or error (APP_ERROR). */
    while (get_sd_app_state() < APP_IDLE)
    {
      /* Maintain system services */
      SYS_FS_Tasks();

      /* Maintain Device Drivers */
      DRV_SDSPI_Tasks(sysObj.drvSDSPI0);

      /* Maintain the application's state machine. */
      APP_Tasks();

      if (get_sd_app_state() != APP_ERROR)
        error = false;
    }
  }

  /*re init application's state machine*/
  set_sd_app_state(APP_START);

  sd_card_go_to_sleep();

  return error;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize(void)
{
  /* Place the App state machine in its initial state. */
  appData.state = APP_START;
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks(void)
{
  static char               currentfilename[32]      = "data.csv";
  static uint16_t           mountUnmount_try_counter = 0;
  char                      csvline[168]             = {'\0'};
  SYS_FS_RESULT             res;
  SYS_FS_FORMAT_PARAM       opt = {0};
  uint8_t CACHE_ALIGN       work[512];

  /* Check the application's current state. */
  switch (appData.state)
  {
  case APP_START :
    appData.state = APP_MOUNT_DISK;
    break;
  case APP_MOUNT_DISK :
    SEGGER_SYSVIEW_RecordVoid(ID_SYSVIEW_E_MOUNT_SD_CARD);
    if (SYS_FS_Mount(SDCARD_DEV_NAME, SDCARD_MOUNT_NAME, FAT, 0, NULL) != 0)
    {
      /* The disk could not be mounted. Try
       * mounting again until success. */
      appData.state = APP_MOUNT_DISK;
      mountUnmount_try_counter++;
      if (mountUnmount_try_counter >= MOUNTUNMOUNT_MAX_TRY)
      {
        appData.state            = APP_ERROR;
        mountUnmount_try_counter = 0;
      }
    }
    else
    {
      appData.state = APP_SET_CURRENT_DRIVE;

      mountUnmount_try_counter = 0;
    }
    break;

  case APP_SET_CURRENT_DRIVE :
    if (SYS_FS_CurrentDriveSet(SDCARD_MOUNT_NAME) == SYS_FS_RES_FAILURE)
    {
      /* Error while setting current drive */
      appData.state = APP_ERROR;
    }
    else
    {
      /* Open a file for reading. */
      appData.state = APP_OPEN_FIRST_FILE;
    }
    break;

  case APP_OPEN_FIRST_FILE :
    SEGGER_SYSVIEW_RecordVoid(ID_SYSVIEW_E_OPEN_SD_CARD);

    appData.fileHandle = SYS_FS_FileOpen(currentfilename, (SYS_FS_FILE_OPEN_APPEND));

    /* Will open a file in write mode. It creates the file if it does not exist */
    if (appData.fileHandle == SYS_FS_HANDLE_INVALID)
    {
      /* Could not open the file. Error out*/
      appData.state = APP_ERROR;
    }
    else
    {
      /* Is fileHandle a new file? */
      if (!SYS_FS_FileSize(appData.fileHandle))
      {
        appData.state = APP_WRITE_FIRST_LINE_TO_FILE;
      }
      else
      {
        appData.state = APP_WRITE_TO_FILE;
      }
    }
    break;
  case APP_WRITE_FIRST_LINE_TO_FILE :
    // snprintf(csvline, 128, "T(degC);H(%%);Luminosity(lux)\r\n");
    SEGGER_SYSVIEW_Print("T(degC);H(%);Luminosity(lux)\r\n");
    if (SYS_FS_FileStringPut(appData.fileHandle, "T(degC);H(%%);Luminosity(lux)\r\n") == -1)
    {
      /* Write was not successful. Close the file
       * and error out.*/
      SYS_FS_FileClose(appData.fileHandle);

      appData.state = APP_ERROR;
      break;
    }
    appData.state = APP_WRITE_TO_FILE;
    break;

  case APP_WRITE_TO_FILE : ;
    /* Product CSV line */
    SEGGER_SYSVIEW_RecordVoid(ID_SYSVIEW_E_WRITE_SD_CARD);

    // Read FIFO and write to SD until measures_FIFO is empty
    uint16_t ml_size = measures_logger_get_size();
    for (int i = 0; i < ml_size; i++)
    {
      memset(csvline, 0, sizeof(csvline));
      /* Read measure from stack */
      bool ml_read_err = measures_logger_read_CSV(csvline);
	  #ifdef DEBUG
	    printf("'%s'\n", csvline);
    #endif
      if (ml_read_err == MEASURES_LOGGER_RES_FAILURE)
      {
        MEASURES_LOGGER_ERROR ml_err = measures_logger_get_error();

        if (ml_err == MEASURES_LOGGER_ERROR_OK)
        {
          /* Do nothing */
        }
        else if (ml_err == MEASURES_LOGGER_NOTHING_TO_READ)
        {
          break;  // Stop reading measures logger, it is empty.
        }
        else
        {
          appData.state = APP_ERROR;
          break;
        }
      }

      strcat(csvline, "\r\n");
      SEGGER_SYSVIEW_Print(csvline);

      if (SYS_FS_FileStringPut(appData.fileHandle, csvline) == SYS_FS_RES_FAILURE)
      {
        /* Write was not successful. Close the file
         * and error out.*/
        SYS_FS_FileClose(appData.fileHandle);
        appData.state = APP_ERROR;
        break;
      }
    }

    if (appData.state != APP_ERROR)
    {
      /* all measures was read from stack and write to SD card */
      appData.state = APP_CLOSE_FILE;
    }
    break;
  case APP_CLOSE_FILE :
    SEGGER_SYSVIEW_RecordVoid(ID_SYSVIEW_E_CLOSE_SD_CARD);

    SYS_FS_FileClose(appData.fileHandle);

    /* The test was successful. Lets idle. */
    appData.state = APP_UNMOUNT_DISK;
    break;

  case APP_UNMOUNT_DISK :
    SEGGER_SYSVIEW_RecordVoid(ID_SYSVIEW_E_UNMOUNT_SD_CARD);

    if (SYS_FS_Unmount(SDCARD_MOUNT_NAME) != 0)
    {
      /* The disk could not be un mounted. Try
       * un mounting again untill success. */

      appData.state = APP_UNMOUNT_DISK;
      mountUnmount_try_counter++;
      if (mountUnmount_try_counter >= MOUNTUNMOUNT_MAX_TRY)
      {
        appData.state = APP_ERROR;
      }
    }
    else
    {
      /* UnMount was successful. Mount the disk again */
      appData.state            = APP_IDLE;
      mountUnmount_try_counter = 0;
    }
    break;

  case APP_FORMAT_DRIVE :
    opt.fmt     = SYS_FS_FORMAT_FAT;
    opt.au_size = 0;

    res         = SYS_FS_DriveFormat(SDCARD_MOUNT_NAME, &opt, (void *)work, 512);
    if (res == SYS_FS_RES_FAILURE)
    {
      appData.state = APP_ERROR;
    }
    else
    {
      appData.state = APP_UNMOUNT_DISK;
    }
    break;

  case APP_IDLE :
    /* The application comes here when the demo has completed
     * successfully. */
    mountUnmount_try_counter = 0;
    break;

  case APP_ERROR :
    /* The application comes here when the demo has failed. Close the opened file */

    // Try to close the file.
    SYS_FS_FileClose(appData.fileHandle);

    // If no error in mounting or unmounting the disk then try to unmount it.
    if (mountUnmount_try_counter < MOUNTUNMOUNT_MAX_TRY)
      appData.state = APP_UNMOUNT_DISK;
    else
      appData.state = APP_IDLE;
    break;

  default :
    break;
  }
}

APP_STATES get_sd_app_state() { return appData.state; }
void       set_sd_app_state(APP_STATES status) { appData.state = status; }

void sd_card_go_to_sleep()
{
  SEGGER_SYSVIEW_RecordVoid(ID_SYSVIEW_E_SLEEP_SD_CARD);
  SpiDeInit();

  // Set pin direction SD card ChipSelect
  sd_card_set_CS_pin_as_lowpower();

  sd_card_power_off();
}

void sd_card_wake_from_sleep(void)
{
  SEGGER_SYSVIEW_RecordVoid(ID_SYSVIEW_E_WAKE_UP_SD_CARD);
  SpiInit();

  // Set pin direction SD card ChipSelect
  sd_card_set_CS_pin_as_output();

  sd_card_power_on();

  // Wait for busy
  delay_ms(1);
}