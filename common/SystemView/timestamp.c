#include <hal_timer.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>
#include <hpl_tc_base.h>
#include <hpl_time_measure.h>
#include <peripheral_clk_config.h>

#include "SEGGER_SYSVIEW.h"

struct timer_descriptor      TIMER_0;

static bool wake_up_tc_timestamp = false;

extern unsigned int SEGGER_SYSVIEW_TickCnt;

/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
void TIMER_0_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, TC4);
	_gclk_enable_channel(TC4_GCLK_ID, CONF_GCLK_TC4_SRC);

	timer_init(&TIMER_0, TC4, _tc_get_timer());
}

void increment_timestamp(const struct timer_task* const timer_task)
{
    (void)timer_task;
    SEGGER_SYSVIEW_TickCnt++;
    DEBUG_SEGGER_SYSVIEW_RecordEnterISR();
    wake_up_tc_timestamp = true;
    DEBUG_SEGGER_SYSVIEW_RecordExitISR();
}

bool get_tc_timestamp_status()
{
    return wake_up_tc_timestamp;
}

void reset_tc_timestamp_status()
{
    wake_up_tc_timestamp = false;
}

struct timer_task task_timestamp = {{0}, 0, 1, &increment_timestamp, TIMER_TASK_REPEAT};

void start_timestamp()
{
    timer_start(&TIMER_0);
    timer_add_task(&TIMER_0, &task_timestamp);
}

/*********************************************************************
 *
 *       SEGGER_SYSVIEW_X_GetTimestamp()
 *
 * Function description
 *   Returns the current timestamp in ticks using the TC6 32bits
 *   count and the TC6 counter.
 *
 * Return value
 *   The current timestamp.
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
