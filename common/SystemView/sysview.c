#include "sysview.h"

#include <stdint.h>

#include "timestamp.h"

int  main();
void delay_ms(const uint16_t ms);

void _cbSendIsrName(void)
{
    SEGGER_SYSVIEW_SendSysDesc("I#18=WDT_ISR, I#19=RTC_ISR, I#20=EIC_ISR ,I#35=TC4_ISR");
}

void SYSVIEW_init()
{
#ifdef SEGGER_SYSTEM_VIEW
    start_timestamp();
#endif
    DEBUG_SEGGER_SYSVIEW_Conf();  /* Configure and initialize SystemView  */
    DEBUG_SEGGER_SYSVIEW_Start(); /* Starts SystemView recording*/
    DEBUG_SYSVIEW_AddTask(main, "main", 10);
    DEBUG_SEGGER_SYSVIEW_OnTaskStartExec((U32) main);
    DEBUG_SYSVIEW_AddDataRegister(TEMP_ID, "temperature", SEGGER_SYSVIEW_TYPE_U32, "°C");
    DEBUG_SYSVIEW_AddDataRegister(HUM_ID, "humidity", SEGGER_SYSVIEW_TYPE_U32, "%");
}
