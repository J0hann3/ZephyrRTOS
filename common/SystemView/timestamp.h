#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <stdbool.h>

void TIMER_0_init(void);
bool get_tc_timestamp_status();
void reset_tc_timestamp_status();
void start_timestamp();

#endif // TIMESTAMP_H_
