#ifndef CALENDAR_H

#include <stdint.h>

extern struct calendar_descriptor CALENDAR_0;

void CALENDAR_0_init(void);
bool get_wake_up_calendar();
void clear_wake_up_calendar();

#endif