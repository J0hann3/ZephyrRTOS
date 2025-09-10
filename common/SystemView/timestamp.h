#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <stdbool.h>
#include <stdint.h>
#include "SEGGER_SYSVIEW.h"

void TIMER_0_init(void);
bool get_tc_timestamp_status();
void reset_tc_timestamp_status();
void start_timestamp();
U32 get_timestamp(void);
bool is_delay_reach(uint32_t delay, uint32_t start_time);

#endif // TIMESTAMP_H_
