#include "temp_hum_sensor.h"
#include "lum_sensor.h"
#include "measures_logger.h"
#include "sd_card_driver/definitions.h"
#include "tools.h"
#include "calendar.h"
#include "SEGGER_SYSVIEW_Conf.h"
#include "SEGGER_SYSVIEW.h"
#include "work_queue.h"
#include "measures_logger.h"
#include "timestamp.h"

measure_t current_measure = {.brightness = UINT16_MAX,
						.humidity = UINT16_MAX,
						.temperature = UINT16_MAX,
						.TEMP_HUM_SENSOR_EN = 1,
						.LUM_SENSOR_EN = 1};

static void BoardInitPeriph(void);

int main(void)
{							
	system_init();
	BoardInitPeriph();
	spi_go_to_sleep();

	sd_card_power_off();
	SYS_Initialize(NULL);
	measures_logger_init(1);

	SYSVIEW_init();
	
	wq_init();
	if (current_measure.LUM_SENSOR_EN)
		wq_enqueue(measure_light_sensor, NULL);
	if (current_measure.TEMP_HUM_SENSOR_EN)
		wq_enqueue(measure_temp_hum_sensor, NULL);

	while (1)
	{
		while (wq_not_empty())
		{
			wq_process();
		}
		measures_logger_write(&current_measure);
		if (measures_logger_count() >= measures_logger_get_size())
			sd_card_write(NULL);

		if (!wq_not_empty())
		{
			DEBUG_SEGGER_SYSVIEW_OnIdle();
			_go_to_sleep();
			DEBUG_SEGGER_SYSVIEW_OnTaskStartExec((U32)main);
		}
	}
	return 0;
}

static void BoardInitPeriph(void)
{
	// inputs
	gpio_set_pin_direction(NFC_FD, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(NFC_FD, GPIO_PULL_UP);

	// test conso
	gpio_set_pin_direction(PUSH_BUTTON, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(PUSH_BUTTON, GPIO_PULL_DOWN);

	gpio_set_pin_direction(PUSH_REVERSE_BUTTON, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(PUSH_REVERSE_BUTTON, GPIO_PULL_DOWN);

	// test conso
	gpio_set_pin_direction(DATA_PIR_RX, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(DATA_PIR_RX, GPIO_PULL_DOWN);

	gpio_set_pin_direction(VCC_3V3_ADC, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(VCC_3V3_ADC, GPIO_PULL_DOWN);

	// outputs

	gpio_set_pin_direction(RED_LED_CMD, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(RED_LED_CMD, true);

	gpio_set_pin_direction(ORANGE_LED_CMD, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(ORANGE_LED_CMD, true);

	gpio_set_pin_direction(GREEN_LED_CMD, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(GREEN_LED_CMD, false);

	gpio_set_pin_direction(RED_REV_LED_CMD, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(RED_REV_LED_CMD, true);

	gpio_set_pin_direction(GREEN_REV_LED_CMD, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(GREEN_REV_LED_CMD, true);

	gpio_set_pin_direction(NFC_VCC, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(NFC_VCC, false);

	gpio_set_pin_direction(VCC_SENSORS_EN, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(VCC_SENSORS_EN, true);

	gpio_set_pin_direction(CMD_BUZZER, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(CMD_BUZZER, false);

	gpio_set_pin_direction(CONFIG_PIR_TX, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(CONFIG_PIR_TX, false);

	gpio_set_pin_direction(ZMOD_RST, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(ZMOD_RST, true);

	gpio_set_pin_direction(SX126X_SS, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(SX126X_SS, true);

	gpio_set_pin_direction(VCC_ADC_CMD, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(VCC_ADC_CMD, false);

	// Micro
	gpio_set_pin_level(VCC_MICRO_EN, true);
	gpio_set_pin_direction(VCC_MICRO_EN, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(VCC_MICRO_EN, GPIO_PIN_FUNCTION_OFF);
	gpio_set_pin_direction(MIC_PDM_DATA, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(MIC_PDM_DATA, GPIO_PULL_DOWN);
	gpio_set_pin_direction(MIC_PDM_CLK, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(MIC_PDM_CLK, GPIO_PULL_DOWN);

	// Magnet detector
	gpio_set_pin_direction(MAGNET_DETECTOR_VCC, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(MAGNET_DETECTOR_VCC, false);
}
