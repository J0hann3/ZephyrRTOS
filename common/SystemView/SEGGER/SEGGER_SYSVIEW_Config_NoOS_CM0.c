/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*            (c) 1995 - 2024 SEGGER Microcontroller GmbH             *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       SEGGER SystemView * Real-time application analysis           *
*                                                                    *
**********************************************************************
*                                                                    *
* All rights reserved.                                               *
*                                                                    *
* SEGGER strongly recommends to not make any changes                 *
* to or modify the source code of this software in order to stay     *
* compatible with the SystemView and RTT protocol, and J-Link.       *
*                                                                    *
* Redistribution and use in source and binary forms, with or         *
* without modification, are permitted provided that the following    *
* condition is met:                                                  *
*                                                                    *
* o Redistributions of source code must retain the above copyright   *
*   notice, this condition and the following disclaimer.             *
*                                                                    *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND             *
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,        *
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF           *
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
* DISCLAIMED. IN NO EVENT SHALL SEGGER Microcontroller BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR           *
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT  *
* OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;    *
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF      *
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT          *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE  *
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH   *
* DAMAGE.                                                            *
*                                                                    *
**********************************************************************
*                                                                    *
*       SystemView version: 3.60e                                    *
*                                                                    *
**********************************************************************
-------------------------- END-OF-HEADER -----------------------------

File    : SEGGER_SYSVIEW_Config_NoOS.c
Purpose : Sample setup configuration of SystemView without an OS for Cortex-M0.
Revision: $Rev: 18540 $
*/
#include "SEGGER_SYSVIEW.h"
#include "SEGGER_SYSVIEW_Conf.h"

#include <peripheral_clk_config.h>
#include <hpl_time_measure.h>
#include "driver_init.h"

// SystemcoreClock can be used in most CMSIS compatible projects.
// In non-CMSIS projects define SYSVIEW_CPU_FREQ.
extern unsigned int SEGGER_SYSVIEW_TickCnt;

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/
/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
// The application name to be displayed in SystemViewer
#define SYSVIEW_APP_NAME        "Nxg_court"

// The target device name
#define SYSVIEW_DEVICE_NAME     "nxg_court atmel_sam0 arm"

// Frequency of the timestamp. Must match SEGGER_SYSVIEW_Conf.h
#define SYSVIEW_TIMESTAMP_FREQ  (CONF_GCLK_TC4_FREQUENCY)

// System Frequency. SystemcoreClock is used in most CMSIS compatible projects.
#define SYSVIEW_CPU_FREQ        (CONF_CPU_FREQUENCY)

// The lowest RAM address used for IDs (pointers)
#define SYSVIEW_RAM_BASE        (0x10000000)

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/

/********************************************************************* 
*
*       _cbSendSystemDesc()
*
*  Function description
*    Sends SystemView description strings.
*/
static void _cbSendSystemDesc(void) {
  SEGGER_SYSVIEW_SendSysDesc("N="SYSVIEW_APP_NAME",O=NoOS_NxgCourt,D="SYSVIEW_DEVICE_NAME);
  SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");

  // Data plot register
  SEGGER_SYSVIEW_DATA_REGISTER TempPlot;
  
  TempPlot.ID = 0;
  TempPlot.sName = "Temp sensor";
  TempPlot.DataType = SEGGER_SYSVIEW_TYPE_U32;
  TempPlot.Offset = 0;
  TempPlot.RangeMin = -40;
  TempPlot.RangeMax = 1250;
  TempPlot.ScalingFactor = 1;
  TempPlot.sUnit = "°C";

  SEGGER_SYSVIEW_RegisterData(&TempPlot);

  SEGGER_SYSVIEW_DATA_REGISTER HumPlot;
  
  HumPlot.ID = 1;
  HumPlot.sName = "Humidity sensor";
  HumPlot.DataType = SEGGER_SYSVIEW_TYPE_U32;
  HumPlot.Offset = 0;
  HumPlot.RangeMin = 0;
  HumPlot.RangeMax = 1000;
  HumPlot.ScalingFactor = 1;
  HumPlot.sUnit = "%RH";

  SEGGER_SYSVIEW_RegisterData(&HumPlot);

  // Create task main
	SEGGER_SYSVIEW_OnTaskCreate(MAIN_TASK);
  
  SEGGER_SYSVIEW_TASKINFO Info = {0};
	Info.TaskID = (U32)MAIN_TASK;
	Info.sName = MAIN_TASK_NAME;

	SEGGER_SYSVIEW_SendTaskInfo(&Info);
	SEGGER_SYSVIEW_OnTaskStartExec(MAIN_TASK);
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/
void increment_timestamp(const struct timer_task *const timer_task)
{
  SEGGER_SYSVIEW_TickCnt++;
  SEGGER_SYSVIEW_RecordEnterISR();
  wake_up_tc_timestamp = true;
  SEGGER_SYSVIEW_RecordExitISR();
}

struct timer_task task_timestamp = {{0}, 0, 1, &increment_timestamp, TIMER_TASK_REPEAT};

void SEGGER_SYSVIEW_Conf(void) {
  timer_start(&TIMER_0);
  timer_add_task(&TIMER_0, &task_timestamp);

  SEGGER_SYSVIEW_Init(SYSVIEW_TIMESTAMP_FREQ, SYSVIEW_CPU_FREQ, 
                      0, _cbSendSystemDesc);
  SEGGER_SYSVIEW_SetRAMBase(SYSVIEW_RAM_BASE);
}

/*********************************************************************
*
*       SEGGER_SYSVIEW_X_GetTimestamp()
*
* Function description
*   Returns the current timestamp in ticks using the system tick 
*   count and the SysTick counter.
*   All parameters of the SysTick have to be known and are set via
*   configuration defines on top of the file.
*
* Return value
*   The current timestamp.
*
* Additional information
*   SEGGER_SYSVIEW_X_GetTimestamp is always called when interrupts are
*   disabled. Therefore locking here is not required.
*/
U32 SEGGER_SYSVIEW_X_GetTimestamp(void) {
  static U32 OldTickCount = 0;
  static U32 OldCycles = 0;
  U32 Cycles;
  U32 TickCount;
  U32 CyclesPerTick;
  //
  // Get the cycles of the current system tick.
  // SysTick is down-counting, subtract the current value from the number of cycles per tick.
  //
  timer_get_clock_cycles_in_tick(&TIMER_0, &CyclesPerTick);
  Cycles = hri_tccount32_read_COUNT_COUNT_bf(TIMER_0.device.hw);
  //
  // Get the system tick count.
  //
  TickCount = SEGGER_SYSVIEW_TickCnt;

  if (Cycles < OldCycles && TickCount <= OldTickCount)
  {
    TickCount = OldTickCount + 1;
  }
  else if (TickCount < OldTickCount)
  {
    TickCount = OldTickCount;
  }

  // SEGGER_RTT_printf(0, "Cycles: %d, OldCycles: %d, ticks: %d, OldTicks: %d\n",Cycles, OldCycles, TickCount, OldTickCount);
  OldTickCount = TickCount;
  OldCycles = Cycles;

  return Cycles + TickCount * CyclesPerTick;
}

/*********************************************************************
*
*       SEGGER_SYSVIEW_X_GetInterruptId()
*
* Function description
*   Return the currently active interrupt Id,
*   which ist the active vector taken from IPSR[5:0].
*
* Return value
*   The current currently active interrupt Id.
*
* Additional information
*   This function is not used by default, as the active vector can be 
*   read from ICSR instead on Cortex-M0.
*   For Cortex-M0+ devices, change SEGGER_SYSVIEW_GET_INTERRUPT_ID
*   in SEGGER_SYSVIEW_Conf.h to call this function instead.
*/
U32 SEGGER_SYSVIEW_X_GetInterruptId(void) {
  U32 Id;

  __asm volatile ("mrs %0, ipsr"
                  : "=r" (Id)
                  );
  Id &= 0x3F;

  return Id;
}

/*************************** End of file ****************************/
