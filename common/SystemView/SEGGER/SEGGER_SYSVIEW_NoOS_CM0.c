/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*            (c) 1995 - 2021 SEGGER Microcontroller GmbH             *
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
*       SystemView version: 3.32                                    *
*                                                                    *
**********************************************************************
-------------------------- END-OF-HEADER -----------------------------

File    : SEGGER_SYSVIEW_Config_NoOS.c
Purpose : Sample setup configuration of SystemView without an OS for Cortex-M0.
Revision: $Rev: 18540 $
*/

#include <stdbool.h>
#include <stdlib.h>

#include "SEGGER_SYSVIEW.h"

/*********************************************************************
 *
 *       Defines, fixed
 *
 ***********************************************************************/

#ifndef SYSVIEW_NUM_TASKS
    #define SYSVIEW_NUM_TASKS 16
#endif

static SEGGER_SYSVIEW_TASKINFO _aTasks[SYSVIEW_NUM_TASKS];
static int                     _NumTasks;

#ifndef SYSVIEW_NUM_DATA_REGISTER
    #define SYSVIEW_NUM_DATA_REGISTER 16
#endif

static SEGGER_SYSVIEW_DATA_REGISTER _aDataRegister[SYSVIEW_NUM_DATA_REGISTER];
static int                          _NumDataRegister;

/*********************************************************************
 *
 *       _cbSendDataRegister()
 *
 *  Function description
 *    Sends SystemView Data Plot
 */
static void _cbSendDataRegister(void)
{
    for (int n = 0; n < _NumDataRegister; n++)
    {
        SEGGER_SYSVIEW_RegisterData(&_aDataRegister[n]);
    }
}

void __attribute__((weak)) _cbSendIsrName(void);

// Add an offset of 16 for the interrupt number on SAMD21
void _cbSendIsrName(void)
{
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");
}

/*********************************************************************
 *
 *       _cbSendSystemDesc()
 *
 *  Function description
 *    Sends SystemView description strings.
 */
static void _cbSendSystemDesc(void)
{
    SEGGER_SYSVIEW_SendSysDesc("N=" SYSVIEW_APP_NAME ",O=" SYSVIEW_PROJECT_NAME ",D=" SYSVIEW_DEVICE_NAME);
    _cbSendIsrName();
    _cbSendDataRegister();
}

/*********************************************************************
 *
 *       _cbSendTaskList()
 *
 *  Function description
 *    Sends SystemView Task List
 */
static void _cbSendTaskList(void)
{
    for (int n = 0; n < _NumTasks; n++)
    {
        SEGGER_SYSVIEW_SendTaskInfo(&_aTasks[n]);
    }
}

/*********************************************************************
 *
 *       Static data
 *
 **********************************************************************
 */

static const SEGGER_SYSVIEW_OS_API _NoOSAPI = {(void*) 0, _cbSendTaskList};

/*********************************************************************
 *
 *       Global functions
 *
 **********************************************************************
 */

/*********************************************************************
 *
 *       SYSVIEW_AddTask()
 *
 *  Function description
 *    Helper function to register Data Plot in a NoOS system
 * @Id id of the data reigster, used to send this data
 * @sName name of the data register
 * @DataType Type of data SEGGER_SYSVIEW_TYPE_U32,  SEGGER_SYSVIEW_TYPE_I32, SEGGER_SYSVIEW_TYPE_FLOAT
 * @sUnit name of the unit for the data
 */
void SYSVIEW_AddDataRegister(U32 Id, const char* sName, U16 DataType, const char* sUnit)
{
    int n;

    if (_NumDataRegister > SYSVIEW_NUM_DATA_REGISTER)
    {
        return;
    }
    n = _NumDataRegister;
    _NumDataRegister++;

    _aDataRegister[n].ID            = Id;
    _aDataRegister[n].sName         = sName;
    _aDataRegister[n].DataType      = DataType;
    _aDataRegister[n].Offset        = 0;
    _aDataRegister[n].RangeMin      = 0;
    _aDataRegister[n].RangeMax      = 0;
    _aDataRegister[n].ScalingFactor = 1;
    _aDataRegister[n].sUnit         = sUnit;
    if (SEGGER_SYSVIEW_IsStarted() > 0)
    {
        SEGGER_SYSVIEW_RegisterData(&_aDataRegister[n]);
    }
}

/*********************************************************************
 *
 *       SYSVIEW_AddTask()
 *
 *  Function description
 *    Helper function for task creation in a NoOS system
 * @pTask function pointer to task to register
 * @sNAme Name of the task register
 * @priority of the task(Not usefull for No OS)
 */
void SYSVIEW_AddTask(void* pTask, const char* sName, U32 Prio)
{
    int n;

    SEGGER_SYSVIEW_OnTaskCreate((U32) pTask);

    if (_NumTasks >= SYSVIEW_NUM_TASKS)
    {
        return;
    }
    n = _NumTasks;
    _NumTasks++;

    _aTasks[n].TaskID    = (U32) pTask;
    _aTasks[n].sName     = sName;
    _aTasks[n].Prio      = Prio;
    _aTasks[n].StackBase = 0;
    _aTasks[n].StackSize = 0;
    if (SEGGER_SYSVIEW_IsStarted() > 0)
    {
        SEGGER_SYSVIEW_SendTaskInfo(&_aTasks[n]);
    }
}

void SEGGER_SYSVIEW_Conf(void)
{
    SEGGER_SYSVIEW_Init(SYSVIEW_TIMESTAMP_FREQ, SYSVIEW_CPU_FREQ,
                        &_NoOSAPI, _cbSendSystemDesc);
    SEGGER_SYSVIEW_SetRAMBase(SYSVIEW_RAM_BASE);
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
U32 SEGGER_SYSVIEW_X_GetInterruptId(void)
{
    U32 Id;

    __asm volatile("mrs %0, ipsr"
                   : "=r"(Id));
    Id &= 0x3F;

    return Id;
}

/*************************** End of file ****************************/
