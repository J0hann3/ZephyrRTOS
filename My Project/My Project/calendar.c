#include <hal_calendar.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>
#include <peripheral_clk_config.h>
#include "calendar.h"

#ifdef DEBUG
#include "printf.h"
#endif

struct calendar_descriptor CALENDAR_0;

bool wake_up_calendar = false;

static struct calendar_alarm CALENDAR_0_task_wdt0, CALENDAR_0_task_wdt1, CALENDAR_0_task_wdt2, CALENDAR_0_task_wdt3, CALENDAR_0_task_wdt4, CALENDAR_0_task_wdt5;

static void CALENDAR_0_task_wdt_callback(struct calendar_descriptor *const descr);

static void calendar0_init()
{
      // TASK 1 = WDT Timer = 10 seconds
  CALENDAR_0_task_wdt0.cal_alarm.datetime.time.sec = 0;  // RTC Count=10s
  CALENDAR_0_task_wdt0.cal_alarm.option            = CALENDAR_ALARM_MATCH_SEC;
  CALENDAR_0_task_wdt0.cal_alarm.mode              = REPEAT;
  CALENDAR_0_task_wdt1.cal_alarm.datetime.time.sec = 10;  // RTC Count=10s
  CALENDAR_0_task_wdt1.cal_alarm.option            = CALENDAR_ALARM_MATCH_SEC;
  CALENDAR_0_task_wdt1.cal_alarm.mode              = REPEAT;
  CALENDAR_0_task_wdt2.cal_alarm.datetime.time.sec = 20;  // RTC Count=10s
  CALENDAR_0_task_wdt2.cal_alarm.option            = CALENDAR_ALARM_MATCH_SEC;
  CALENDAR_0_task_wdt2.cal_alarm.mode              = REPEAT;
  CALENDAR_0_task_wdt3.cal_alarm.datetime.time.sec = 30;  // RTC Count=10s
  CALENDAR_0_task_wdt3.cal_alarm.option            = CALENDAR_ALARM_MATCH_SEC;
  CALENDAR_0_task_wdt3.cal_alarm.mode              = REPEAT;
  CALENDAR_0_task_wdt4.cal_alarm.datetime.time.sec = 40;  // RTC Count=10s
  CALENDAR_0_task_wdt4.cal_alarm.option            = CALENDAR_ALARM_MATCH_SEC;
  CALENDAR_0_task_wdt4.cal_alarm.mode              = REPEAT;
  CALENDAR_0_task_wdt5.cal_alarm.datetime.time.sec = 50;  // RTC Count=10s
  CALENDAR_0_task_wdt5.cal_alarm.option            = CALENDAR_ALARM_MATCH_SEC;
  CALENDAR_0_task_wdt5.cal_alarm.mode              = REPEAT;

  calendar_set_alarm(&CALENDAR_0, &CALENDAR_0_task_wdt0, CALENDAR_0_task_wdt_callback);
  calendar_set_alarm(&CALENDAR_0, &CALENDAR_0_task_wdt1, CALENDAR_0_task_wdt_callback);
  calendar_set_alarm(&CALENDAR_0, &CALENDAR_0_task_wdt2, CALENDAR_0_task_wdt_callback);
  calendar_set_alarm(&CALENDAR_0, &CALENDAR_0_task_wdt3, CALENDAR_0_task_wdt_callback);
  calendar_set_alarm(&CALENDAR_0, &CALENDAR_0_task_wdt4, CALENDAR_0_task_wdt_callback);
  calendar_set_alarm(&CALENDAR_0, &CALENDAR_0_task_wdt5, CALENDAR_0_task_wdt_callback);

  calendar_enable(&CALENDAR_0);
}

void CALENDAR_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBA, RTC);
	_gclk_enable_channel(RTC_GCLK_ID, CONF_GCLK_RTC_SRC);
}

void CALENDAR_0_init(void)
{
	CALENDAR_0_CLOCK_init();
	calendar_init(&CALENDAR_0, RTC);
	struct calendar_date new_date;
	struct calendar_time new_time;
	
	new_date.year  = 2025;
	new_date.month = 04;
	new_date.day   = 16;

	calendar_set_date(&CALENDAR_0, &new_date);

	new_time.hour = 15;
	new_time.min  = 41;
	new_time.sec  = 1;

	calendar_set_time(&CALENDAR_0, &new_time);
    calendar0_init();
}

static void CALENDAR_0_task_wdt_callback(struct calendar_descriptor *const descr)
{
    SEGGER_SYSVIEW_RecordEnterISR();
	(void)descr;
    wake_up_calendar = true;
    SEGGER_SYSVIEW_RecordExitISR();
    return;
}