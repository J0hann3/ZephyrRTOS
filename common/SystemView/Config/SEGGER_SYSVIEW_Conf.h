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

File    : SEGGER_SYSVIEW_Conf.h
Purpose : SEGGER SystemView configuration file.
          Set defines which deviate from the defaults (see SEGGER_SYSVIEW_ConfDefaults.h) here.
Revision: $Rev: 21292 $

Additional information:
  Required defines which must be set are:
    SEGGER_SYSVIEW_GET_TIMESTAMP
    SEGGER_SYSVIEW_GET_INTERRUPT_ID
  For known compilers and cores, these might be set to good defaults
  in SEGGER_SYSVIEW_ConfDefaults.h.

  SystemView needs a (nestable) locking mechanism.
  If not defined, the RTT locking mechanism is used,
  which then needs to be properly configured.
*/

#ifndef SEGGER_SYSVIEW_CONF_H
    #define SEGGER_SYSVIEW_CONF_H

/*********************************************************************
 * TODO: Add your defines here.                                       *
 **********************************************************************
 */
    #include "Global.h"
    #include "SYSVIEW_Config.h"

void SYSVIEW_AddTask(void* pTask, const char* sName, U32 Prio);
void SYSVIEW_AddDataRegister(U32 Id, const char* sName, U16 DataType, const char* sUnit);

/*********************************************************************
 *
 *       Defines
 *
 **********************************************************************
 */

    #ifdef SEGGER_SYSTEM_VIEW
        #define DEBUG_SEGGER_SYSVIEW_Init(SysFreq, CPUFreq, pOSAPI, pfSendSysDesc) \
            SEGGER_SYSVIEW_Init(SysFreq, CPUFreq, pOSAPI, pfSendSysDesc)
        #define DEBUG_SEGGER_SYSVIEW_SetRAMBase(RAMBaseAddress) \
            SEGGER_SYSVIEW_SetRAMBase(RAMBaseAddress)
        #define DEBUG_SEGGER_SYSVIEW_Start() \
            SEGGER_SYSVIEW_Start()
        #define DEBUG_SEGGER_SYSVIEW_Stop() \
            SEGGER_SYSVIEW_Stop()
        #define DEBUG_SEGGER_SYSVIEW_GetSysDesc() \
            SEGGER_SYSVIEW_GetSysDesc()
        #define DEBUG_SEGGER_SYSVIEW_SendTaskList() \
            SEGGER_SYSVIEW_SendTaskList()
        #define DEBUG_SEGGER_SYSVIEW_SendTaskInfo(pInfo) \
            SEGGER_SYSVIEW_SendTaskInfo(pInfo)
        #define DEBUG_SEGGER_SYSVIEW_SendStackInfo(pInfo) \
            SEGGER_SYSVIEW_SendStackInfo(pInfo)
        #define DEBUG_SEGGER_SYSVIEW_SendSysDesc(sSysDesc) \
            SEGGER_SYSVIEW_SendSysDesc(sSysDesc)
        #define DEBUG_SEGGER_SYSVIEW_IsStarted() \
            SEGGER_SYSVIEW_IsStarted()
        #define DEBUG_SEGGER_SYSVIEW_GetChannelID() \
            SEGGER_SYSVIEW_GetChannelID()
        #define DEBUG_SEGGER_SYSVIEW_SampleData(pInfo) \
            SEGGER_SYSVIEW_SampleData(pInfo)
        #define DEBUG_SEGGER_SYSVIEW_RecordVoid(EventId) \
            SEGGER_SYSVIEW_RecordVoid(EventId)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32(EventId, Para0) \
            SEGGER_SYSVIEW_RecordU32(EventId, Para0)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32x2(EventId, Para0, Para1) \
            SEGGER_SYSVIEW_RecordU32x2(EventId, Para0, Para1)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32x3(EventId, Para0, Para1, Para2) \
            SEGGER_SYSVIEW_RecordU32x3(EventId, Para0, Para1, Para2)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32x4(EventId, Para0, Para1, Para2, Para3) \
            SEGGER_SYSVIEW_RecordU32x4(EventId, Para0, Para1, Para2, Para3)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32x5(EventId, Para0, Para1, Para2, Para3, Para4) \
            SEGGER_SYSVIEW_RecordU32x5(EventId, Para0, Para1, Para2, Para3, Para4)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32x6(EventId, Para0, Para1, Para2, Para3, Para4, Para5) \
            SEGGER_SYSVIEW_RecordU32x6(EventId, Para0, Para1, Para2, Para3, Para4, Para5)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32x7(EventId, Para0, Para1, Para2, Para3, Para4, Para5, Para6) \
            SEGGER_SYSVIEW_RecordU32x7(EventId, Para0, Para1, Para2, Para3, Para4, Para5, Para6)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32x8(EventId, Para0, Para1, Para2, Para3, Para4, Para5, Para6, Para7) \
            SEGGER_SYSVIEW_RecordU32x8(EventId, Para0, Para1, Para2, Para3, Para4, Para5, Para6, Para7)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32x9(EventId, Para0, Para1, Para2, Para3, Para4, Para5, Para6, Para7, Para8) \
            SEGGER_SYSVIEW_RecordU32x9(EventId, Para0, Para1, Para2, Para3, Para4, Para5, Para6, Para7, Para8)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32x10(EventId, Para0, Para1, Para2, Para3, Para4, Para5, Para6, Para7, Para8, Para9) \
            SEGGER_SYSVIEW_RecordU32x10(EventId, Para0, Para1, Para2, Para3, Para4, Para5, Para6, Para7, Para8, Para9)
        #define DEBUG_SEGGER_SYSVIEW_RecordString(EventId, pString) \
            SEGGER_SYSVIEW_RecordString(EventId, pString)
        #define DEBUG_SEGGER_SYSVIEW_RecordSystime() \
            SEGGER_SYSVIEW_RecordSystime()
        #define DEBUG_SEGGER_SYSVIEW_RecordEnterISR() \
            SEGGER_SYSVIEW_RecordEnterISR()
        #define DEBUG_SEGGER_SYSVIEW_RecordExitISR() \
            SEGGER_SYSVIEW_RecordExitISR()
        #define DEBUG_SEGGER_SYSVIEW_RecordExitISRToScheduler() \
            SEGGER_SYSVIEW_RecordExitISRToScheduler()
        #define DEBUG_SEGGER_SYSVIEW_RecordEnterTimer(TimerId) \
            SEGGER_SYSVIEW_RecordEnterTimer(TimerId)
        #define DEBUG_SEGGER_SYSVIEW_RecordExitTimer() \
            SEGGER_SYSVIEW_RecordExitTimer()
        #define DEBUG_SEGGER_SYSVIEW_RecordEndCall(EventID) \
            SEGGER_SYSVIEW_RecordEndCall(EventID)
        #define DEBUG_SEGGER_SYSVIEW_RecordEndCallU32(EventID, Para0) \
            SEGGER_SYSVIEW_RecordEndCallU32(EventID, Para0)
        #define DEBUG_SEGGER_SYSVIEW_OnIdle() \
            SEGGER_SYSVIEW_OnIdle()
        #define DEBUG_SEGGER_SYSVIEW_OnTaskCreate(TaskId) \
            SEGGER_SYSVIEW_OnTaskCreate(TaskId)
        #define DEBUG_SEGGER_SYSVIEW_OnTaskTerminate(TaskId) \
            SEGGER_SYSVIEW_OnTaskTerminate(TaskId)
        #define DEBUG_SEGGER_SYSVIEW_OnTaskStartExec(TaskId) \
            SEGGER_SYSVIEW_OnTaskStartExec(TaskId)
        #define DEBUG_SEGGER_SYSVIEW_OnTaskStopExec() \
            SEGGER_SYSVIEW_OnTaskStopExec()
        #define DEBUG_SEGGER_SYSVIEW_OnTaskStartReady(TaskId) \
            SEGGER_SYSVIEW_OnTaskStartReady(TaskId)
        #define DEBUG_SEGGER_SYSVIEW_OnTaskStopReady(TaskId, Cause) \
            SEGGER_SYSVIEW_OnTaskStopReady(TaskId, Cause)
        #define DEBUG_SEGGER_SYSVIEW_MarkStart(MarkerId) \
            SEGGER_SYSVIEW_MarkStart(MarkerId)
        #define DEBUG_SEGGER_SYSVIEW_MarkStop(MarkerId) \
            SEGGER_SYSVIEW_MarkStop(MarkerId)
        #define DEBUG_SEGGER_SYSVIEW_Mark(MarkerId) \
            SEGGER_SYSVIEW_Mark(MarkerId)
        #define DEBUG_SEGGER_SYSVIEW_NameMarker(MarkerId, sName) \
            SEGGER_SYSVIEW_NameMarker(MarkerId, sName)
        #define DEBUG_SEGGER_SYSVIEW_HeapDefine(pHeap, pBase, HeapSize, MetadataSize) \
            SEGGER_SYSVIEW_HeapDefine(pHeap, pBase, HeapSize, MetadataSize)
        #define DEBUG_SEGGER_SYSVIEW_HeapAlloc(pHeap, pUserData, UserDataLen) \
            SEGGER_SYSVIEW_HeapAlloc(pHeap, pUserData, UserDataLen)
        #define DEBUG_SEGGER_SYSVIEW_HeapAllocEx(pHeap, pUserData, UserDataLen, Tag) \
            SEGGER_SYSVIEW_HeapAllocEx(pHeap, pUserData, UserDataLen, Tag)
        #define DEBUG_SEGGER_SYSVIEW_HeapFree(pHeap, pUserData) \
            SEGGER_SYSVIEW_HeapFree(pHeap, pUserData)
        #define DEBUG_SEGGER_SYSVIEW_NameResource(ResourceId, sName) \
            SEGGER_SYSVIEW_NameResource(ResourceId, sName)
        #define DEBUG_SEGGER_SYSVIEW_RegisterData(pInfo) \
            SEGGER_SYSVIEW_RegisterData(pInfo)
        #define DEBUG_SEGGER_SYSVIEW_SendPacket(pPacket, pPayloadEnd, EventId) \
            SEGGER_SYSVIEW_SendPacket(pPacket, pPayloadEnd, EventId)
        #define DEBUG_SEGGER_SYSVIEW_EncodeU32(pPayload, Value) \
            SEGGER_SYSVIEW_EncodeU32(pPayload, Value)
        #define DEBUG_SEGGER_SYSVIEW_EncodeData(pPayload, pSrc, Len) \
            SEGGER_SYSVIEW_EncodeData(pPayload, pSrc, Len)
        #define DEBUG_SEGGER_SYSVIEW_EncodeString(pPayload, s, MaxLen) \
            SEGGER_SYSVIEW_EncodeString(pPayload, s, MaxLen)
        #define DEBUG_SEGGER_SYSVIEW_EncodeId(pPayload, Id) \
            SEGGER_SYSVIEW_EncodeId(pPayload, Id)
        #define DEBUG_SEGGER_SYSVIEW_ShrinkId(Id) \
            SEGGER_SYSVIEW_ShrinkId(Id)
        #define DEBUG_SEGGER_SYSVIEW_RegisterModule(pModule) \
            SEGGER_SYSVIEW_RegisterModule(pModule)
        #define DEBUG_SEGGER_SYSVIEW_RecordModuleDescription(pModule, sDescription) \
            SEGGER_SYSVIEW_RecordModuleDescription(pModule, sDescription)
        #define DEBUG_SEGGER_SYSVIEW_SendModule(ModuleId) \
            SEGGER_SYSVIEW_SendModule(ModuleId)
        #define DEBUG_SEGGER_SYSVIEW_SendModuleDescription() \
            SEGGER_SYSVIEW_SendModuleDescription()
        #define DEBUG_SEGGER_SYSVIEW_SendNumModules() \
            SEGGER_SYSVIEW_SendNumModules()
        #ifndef SEGGER_SYSVIEW_EXCLUDE_PRINTF // Define in project to avoid warnings about variable parameter list
            #define DEBUG_SEGGER_SYSVIEW_PrintfHostEx(s, Options, ...) \
                SEGGER_SYSVIEW_PrintfHostEx(s, Options, __VA_ARGS__)
            #define DEBUG_SEGGER_SYSVIEW_VPrintfHostEx(s, Options, pParamList) \
                SEGGER_SYSVIEW_VPrintfHostEx(s, Options, pParamList)
            #define DEBUG_SEGGER_SYSVIEW_PrintfTargetEx(s, Options, ...) \
                SEGGER_SYSVIEW_PrintfTargetEx(s, Options, __VA_ARGS__)
            #define DEBUG_SEGGER_SYSVIEW_VPrintfTargetEx(s, Options, pParamList) \
                SEGGER_SYSVIEW_VPrintfTargetEx(s, Options, pParamList)
            #define DEBUG_SEGGER_SYSVIEW_PrintfHost(s, ...) \
                SEGGER_SYSVIEW_PrintfHost(s, __VA_ARGS__)
            #define DEBUG_SEGGER_SYSVIEW_VPrintfHost(s, pParamList) \
                SEGGER_SYSVIEW_VPrintfHost(s, pParamList)
            #define DEBUG_SEGGER_SYSVIEW_PrintfTarget(s, ...) \
                SEGGER_SYSVIEW_PrintfTarget(s, __VA_ARGS__)
            #define DEBUG_SEGGER_SYSVIEW_VPrintfTarget(s, pParamList) \
                SEGGER_SYSVIEW_VPrintfTarget(s, pParamList)
            #define DEBUG_SEGGER_SYSVIEW_WarnfHost(s, ...) \
                SEGGER_SYSVIEW_WarnfHost(s, __VA_ARGS__)
            #define DEBUG_SEGGER_SYSVIEW_VWarnfHost(s, pParamList) \
                SEGGER_SYSVIEW_VWarnfHost(s, pParamList)
            #define DEBUG_SEGGER_SYSVIEW_WarnfTarget(s, ...) \
                SEGGER_SYSVIEW_WarnfTarget(s, __VA_ARGS__)
            #define DEBUG_SEGGER_SYSVIEW_VWarnfTarget(s, pParamList) \
                SEGGER_SYSVIEW_VWarnfTarget(s, pParamList)
            #define DEBUG_SEGGER_SYSVIEW_ErrorfHost(s, ...) \
                SEGGER_SYSVIEW_ErrorfHost(s, __VA_ARGS__)
            #define DEBUG_SEGGER_SYSVIEW_VErrorfHost(s, pParamList) \
                SEGGER_SYSVIEW_VErrorfHost(s, pParamList)
            #define DEBUG_SEGGER_SYSVIEW_ErrorfTarget(s, ...) \
                SEGGER_SYSVIEW_ErrorfTarget(s, __VA_ARGS__)
            #define DEBUG_SEGGER_SYSVIEW_VErrorfTarget(s, pParamList) \
                SEGGER_SYSVIEW_VErrorfTarget(s, pParamList)
        #endif
        #define DEBUG_SEGGER_SYSVIEW_Print(s) \
            SEGGER_SYSVIEW_Print(s)
        #define DEBUG_SEGGER_SYSVIEW_Warn(s) \
            SEGGER_SYSVIEW_Warn(s)
        #define DEBUG_SEGGER_SYSVIEW_Error(s) \
            SEGGER_SYSVIEW_Error(s)
        #define DEBUG_SEGGER_SYSVIEW_EnableEvents(EnableMask) \
            SEGGER_SYSVIEW_EnableEvents(EnableMask)
        #define DEBUG_SEGGER_SYSVIEW_DisableEvents(DisableMask) \
            SEGGER_SYSVIEW_DisableEvents(DisableMask)
        #define DEBUG_SEGGER_SYSVIEW_Conf() \
            SEGGER_SYSVIEW_Conf()
        #define DEBUG_SEGGER_SYSVIEW_X_GetTimestamp() \
            SEGGER_SYSVIEW_X_GetTimestamp()
        #define DEBUG_SEGGER_SYSVIEW_X_GetInterruptId() \
            SEGGER_SYSVIEW_X_GetInterruptId()
        #define DEBUG_SEGGER_SYSVIEW_X_StartComm() \
            SEGGER_SYSVIEW_X_StartComm()
        #define DEBUG_SEGGER_SYSVIEW_X_OnEventRecorded(NumBytes) \
            SEGGER_SYSVIEW_X_OnEventRecorded(NumBytes)
        #define DEBUG_SYSVIEW_AddTask(pTask, sName, Prio) \
            SYSVIEW_AddTask(pTask, sName, Prio)
        #define DEBUG_SYSVIEW_AddDataRegister(Id, sName, DataType, sUnit) \
            SYSVIEW_AddDataRegister(Id, sName, DataType, sUnit)

    #else

        #define DEBUG_SEGGER_SYSVIEW_Init(SysFreq, CPUFreq, pOSAPI, pfSendSysDesc)
        #define DEBUG_SEGGER_SYSVIEW_SetRAMBase(RAMBaseAddress)
        #define DEBUG_SEGGER_SYSVIEW_Start()
        #define DEBUG_SEGGER_SYSVIEW_Stop()
        #define DEBUG_SEGGER_SYSVIEW_GetSysDesc()
        #define DEBUG_SEGGER_SYSVIEW_SendTaskList()
        #define DEBUG_SEGGER_SYSVIEW_SendTaskInfo(pInfo)
        #define DEBUG_SEGGER_SYSVIEW_SendStackInfo(pInfo)
        #define DEBUG_SEGGER_SYSVIEW_SendSysDesc(sSysDesc)
        #define DEBUG_SEGGER_SYSVIEW_IsStarted()
        #define DEBUG_SEGGER_SYSVIEW_GetChannelID()
        #define DEBUG_SEGGER_SYSVIEW_SampleData(pInfo)
        #define DEBUG_SEGGER_SYSVIEW_RecordVoid(EventId)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32(EventId, Para0)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32x2(EventId, Para0, Para1)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32x3(EventId, Para0, Para1, Para2)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32x4(EventId, Para0, Para1, Para2, Para3)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32x5(EventId, Para0, Para1, Para2, Para3, Para4)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32x6(EventId, Para0, Para1, Para2, Para3, Para4, Para5)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32x7(EventId, Para0, Para1, Para2, Para3, Para4, Para5, Para6)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32x8(EventId, Para0, Para1, Para2, Para3, Para4, Para5, Para6, Para7)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32x9(EventId, Para0, Para1, Para2, Para3, Para4, Para5, Para6, Para7, Para8)
        #define DEBUG_SEGGER_SYSVIEW_RecordU32x10(EventId, Para0, Para1, Para2, Para3, Para4, Para5, Para6, Para7, Para8, Para9)
        #define DEBUG_SEGGER_SYSVIEW_RecordString(EventId, pString)
        #define DEBUG_SEGGER_SYSVIEW_RecordSystime()
        #define DEBUG_SEGGER_SYSVIEW_RecordEnterISR()
        #define DEBUG_SEGGER_SYSVIEW_RecordExitISR()
        #define DEBUG_SEGGER_SYSVIEW_RecordExitISRToScheduler()
        #define DEBUG_SEGGER_SYSVIEW_RecordEnterTimer(TimerId)
        #define DEBUG_SEGGER_SYSVIEW_RecordExitTimer()
        #define DEBUG_SEGGER_SYSVIEW_RecordEndCall(EventID)
        #define DEBUG_SEGGER_SYSVIEW_RecordEndCallU32(EventID, Para0)
        #define DEBUG_SEGGER_SYSVIEW_OnIdle()
        #define DEBUG_SEGGER_SYSVIEW_OnTaskCreate(TaskId)
        #define DEBUG_SEGGER_SYSVIEW_OnTaskTerminate(TaskId)
        #define DEBUG_SEGGER_SYSVIEW_OnTaskStartExec(TaskId)
        #define DEBUG_SEGGER_SYSVIEW_OnTaskStopExec()
        #define DEBUG_SEGGER_SYSVIEW_OnTaskStartReady(TaskId)
        #define DEBUG_SEGGER_SYSVIEW_OnTaskStopReady(TaskId, Cause)
        #define DEBUG_SEGGER_SYSVIEW_MarkStart(MarkerId)
        #define DEBUG_SEGGER_SYSVIEW_MarkStop(MarkerId)
        #define DEBUG_SEGGER_SYSVIEW_Mark(MarkerId)
        #define DEBUG_SEGGER_SYSVIEW_NameMarker(MarkerId, sName)
        #define DEBUG_SEGGER_SYSVIEW_HeapDefine(pHeap, pBase, HeapSize, MetadataSize)
        #define DEBUG_SEGGER_SYSVIEW_HeapAlloc(pHeap, pUserData, UserDataLen)
        #define DEBUG_SEGGER_SYSVIEW_HeapAllocEx(pHeap, pUserData, UserDataLen, Tag)
        #define DEBUG_SEGGER_SYSVIEW_HeapFree(pHeap, pUserData)
        #define DEBUG_SEGGER_SYSVIEW_NameResource(ResourceId, sName)
        #define DEBUG_SEGGER_SYSVIEW_RegisterData(pInfo)
        #define DEBUG_SEGGER_SYSVIEW_SendPacket(pPacket, pPayloadEnd, EventId)
        #define DEBUG_SEGGER_SYSVIEW_EncodeU32(pPayload, Value)
        #define DEBUG_SEGGER_SYSVIEW_EncodeData(pPayload, pSrc, Len)
        #define DEBUG_SEGGER_SYSVIEW_EncodeString(pPayload, s, MaxLen)
        #define DEBUG_SEGGER_SYSVIEW_EncodeId(pPayload, Id)
        #define DEBUG_SEGGER_SYSVIEW_ShrinkId(Id)
        #define DEBUG_SEGGER_SYSVIEW_RegisterModule(pModule)
        #define DEBUG_SEGGER_SYSVIEW_RecordModuleDescription(pModule, sDescription)
        #define DEBUG_SEGGER_SYSVIEW_SendModule(ModuleId)
        #define DEBUG_SEGGER_SYSVIEW_SendModuleDescription()
        #define DEBUG_SEGGER_SYSVIEW_SendNumModules()
        #ifndef SEGGER_SYSVIEW_EXCLUDE_PRINTF // Define in project to avoid warnings about variable parameter list
            #define DEBUG_SEGGER_SYSVIEW_PrintfHostEx(s, Options, ...)
            #define DEBUG_SEGGER_SYSVIEW_VPrintfHostEx(s, Options, pParamList)
            #define DEBUG_SEGGER_SYSVIEW_PrintfTargetEx(s, Options, ...)
            #define DEBUG_SEGGER_SYSVIEW_VPrintfTargetEx(s, Options, pParamList)
            #define DEBUG_SEGGER_SYSVIEW_PrintfHost(s, ...)
            #define DEBUG_SEGGER_SYSVIEW_VPrintfHost(s, pParamList)
            #define DEBUG_SEGGER_SYSVIEW_PrintfTarget(s, ...)
            #define DEBUG_SEGGER_SYSVIEW_VPrintfTarget(s, pParamList)
            #define DEBUG_SEGGER_SYSVIEW_WarnfHost(s, ...)
            #define DEBUG_SEGGER_SYSVIEW_VWarnfHost(s, pParamList)
            #define DEBUG_SEGGER_SYSVIEW_WarnfTarget(s, ...)
            #define DEBUG_SEGGER_SYSVIEW_VWarnfTarget(s, pParamList)
            #define DEBUG_SEGGER_SYSVIEW_ErrorfHost(s, ...)
            #define DEBUG_SEGGER_SYSVIEW_VErrorfHost(s, pParamList)
            #define DEBUG_SEGGER_SYSVIEW_ErrorfTarget(s, ...)
            #define DEBUG_SEGGER_SYSVIEW_VErrorfTarget(s, pParamList)
        #endif
        #define DEBUG_SEGGER_SYSVIEW_Print(s)
        #define DEBUG_SEGGER_SYSVIEW_Warn(s)
        #define DEBUG_SEGGER_SYSVIEW_Error(s)
        #define DEBUG_SEGGER_SYSVIEW_EnableEvents(EnableMask)
        #define DEBUG_SEGGER_SYSVIEW_DisableEvents(DisableMask)
        #define DEBUG_SEGGER_SYSVIEW_Conf()
        #define DEBUG_SEGGER_SYSVIEW_X_GetTimestamp()
        #define DEBUG_SEGGER_SYSVIEW_X_GetInterruptId()
        #define DEBUG_SEGGER_SYSVIEW_X_StartComm()
        #define DEBUG_SEGGER_SYSVIEW_X_OnEventRecorded(NumBytes)
        #define DEBUG_SYSVIEW_AddTask(pTask, sName, Prio)
        #define DEBUG_SYSVIEW_AddDataRegister(Id, sName, DataType, sUnit)

    #endif

#endif // SEGGER_SYSVIEW_CONF_H

/*************************** End of file ****************************/
