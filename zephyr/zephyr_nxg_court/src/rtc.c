#include <zephyr/drivers/rtc.h>


static void wakeup_handler(const struct device *dev, uint16_t id, void *user_data)
{
    struct rtc_time time = {0};
    rtc_get_time(DEVICE_DT_GET(DT_NODELABEL(rtc)), &time);
	printf("Wake up by alarm. Time: %d:%d:%d\n",time.tm_hour, time.tm_min, time.tm_sec);
}

void test_rtc()
{
    const struct device *rtc = DEVICE_DT_GET(DT_NODELABEL(rtc));
    const struct rtc_time time = {
        .tm_sec = 50,
        .tm_min = 29,
        .tm_hour = 13,
        .tm_mday = 1,
        .tm_mon = 0,
        .tm_year = 121,
        .tm_wday = -1,
        .tm_yday = -1,
        .tm_isdst = -1,
        .tm_nsec = 0,
    };
    const struct rtc_time alarm_time = {
        .tm_sec = 10,
        .tm_min = 29,
        .tm_hour = 13,
        .tm_mday = 1,
        .tm_mon = 0,
        .tm_year = 121,
        .tm_wday = -1,
        .tm_yday = -1,
        .tm_isdst = -1,
        .tm_nsec = 0,
    };
    rtc_set_time(rtc, &time);
    rtc_alarm_set_time(rtc, 0, RTC_ALARM_TIME_MASK_SECOND, &alarm_time);
	rtc_alarm_set_callback(rtc, 0, wakeup_handler, NULL);
}