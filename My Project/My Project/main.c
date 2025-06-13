#include "temp_hum_sensor.h"
#include "lum_sensor.h"
#include "measures_logger.h"
#include "sd_card_driver/definitions.h"
#include "tools.h"
#include "SEGGER_RTT.h"
#include "SEGGER_SYSVIEW_Conf.h"
#include "SEGGER_SYSVIEW.h"

bool wake_up_tc_timestamp = false;

void BoardInitPeriph(void)
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
#define MAIN_TASK 0

int main(void)
{
	struct io_descriptor *i2c_lum = {0};
	struct io_descriptor *i2c_temp = {0};
	measure_t current_measure = {.TEMP_HUM_SENSOR_EN = 1,
								.LUM_SENSOR_EN = 0};
							
	system_init();
	BoardInitPeriph();
	spi_go_to_sleep();

	sd_card_power_off();
	SYS_Initialize(NULL);
	measures_logger_init(1);

	// SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
	// SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_BRIGHT_YELLOW"Welcome here\n"RTT_CTRL_RESET);
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_OnTaskCreate(MAIN_TASK);
	SEGGER_SYSVIEW_OnTaskStartExec(MAIN_TASK);
	
	while (1)
	{
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
		if (wake_up_tc_timestamp)
			wake_up_tc_timestamp = false;
		SEGGER_SYSVIEW_OnIdle();
		_go_to_sleep();
		while (wake_up_tc_timestamp)
		{
			wake_up_tc_timestamp = false;
			_go_to_sleep();
		}
		SEGGER_SYSVIEW_OnTaskStartExec(MAIN_TASK);
	}
	return 0;
}
