#include <hal_calendar.h>

#include "temp_hum_sensor.h"
#include "lum_sensor.h"
#include "measures_logger.h"
#include "sd_card_driver/definitions.h"

int main(void)
{
	struct io_descriptor *i2c_lum = {0};
	struct io_descriptor *i2c_temp = {0};
	measure_t current_measure = {.TEMP_HUM_SENSOR_EN = 1,
								.LUM_SENSOR_EN = 1};
	system_init();

	sd_card_power_off();
	SYS_Initialize(NULL);
	measures_logger_init(1);

	while (1)
	{
		delay_ms(1000);

		calendar_get_date_time(&CALENDAR_0, &current_measure.datetime);
		printf("Current datetime = %u/%u/%u %u:%u:%u : ",
			current_measure.datetime.date.year,
			current_measure.datetime.date.month,
			current_measure.datetime.date.day,
			current_measure.datetime.time.hour,
			current_measure.datetime.time.min,
			current_measure.datetime.time.sec);
		if (current_measure.LUM_SENSOR_EN && init_and_read_lum_sensor(i2c_lum,
					&current_measure.brightness) == 1 )
			return 1;
		if (current_measure.TEMP_HUM_SENSOR_EN && read_temp_sensor(i2c_temp, &current_measure.temperature,
					&current_measure.humidity) == 1 )
			return 1;
		measures_logger_write(&current_measure);
		size_t count = measures_logger_count();
		uint16_t size = measures_logger_get_size();
		if (count >= size)
		{
			SYS_Tasks(USUAL_ACCESS);
		}
	}
	return 0;
}
