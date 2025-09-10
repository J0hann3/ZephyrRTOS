#include "lum_sensor.h"
#include "timestamp.h"
#include "i2c.h"
#include <SEGGER_SYSVIEW.h>
#include "work_queue.h"

#define DELAY_LIGHT_SENSOR_TURN_ON 5 // in ms
#define DELAY_LIGHT_SENSOR_MEASURE 150 // in ms

void turn_on_light_sensor(void *const arg)
{
	light_measure *light = arg;

	gpio_set_pin_direction(VCC_SENSORS_EN, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(VCC_SENSORS_EN, false);
	light->time_start_measure = get_timestamp();
	wq_enqueue(light_sensor_write_command, arg);
}

void turn_off_light_sensor()
{
	gpio_set_pin_direction(VCC_SENSORS_EN, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(VCC_SENSORS_EN, GPIO_PULL_UP);
}

void light_sensor_write_command(void *const arg)
{
	light_measure *light = arg;
	uint8_t   data_config[3] = {0x00, 0x00, 0x08};

	if (!is_delay_reach(DELAY_LIGHT_SENSOR_TURN_ON, light->time_start_measure))
	{
		wq_enqueue(light_sensor_write_command, arg);
		return ;
	}

	i2c_m_sync_get_io_descriptor(&I2C_0, &light->i2c_device);
	i2c_m_sync_enable(&I2C_0);
	i2c_m_sync_set_slaveaddr(&I2C_0, SLAVE_ADDR_LUM, I2C_M_SEVEN);
	
	uint8_t check = io_write(light->i2c_device, data_config, 3);
	if (check != 3)
	{
		wq_enqueue(turn_off_light_sensor, NULL);
		#ifdef DEBUG
		printf("Init i2c error\n");
		#endif
		return;
	}
	light->time_start_measure = get_timestamp();
	wq_enqueue(read_lum_sensor, arg);
}

void read_lum_sensor(void *const arg)
{
	light_measure *light = arg;
	uint8_t   data_read[2];

	if (!is_delay_reach(DELAY_LIGHT_SENSOR_MEASURE, light->time_start_measure))
	{
		wq_enqueue(read_lum_sensor, arg);
		return ;
	}
	
	record_sysview_measure_light_enter();
	i2c_m_sync_set_slaveaddr(&I2C_0, SLAVE_ADDR_LUM, I2C_M_SEVEN);

	if (i2c_m_sync_cmd_read(&I2C_0, 0x04, data_read, 2) != 0)
	{
	#ifdef DEBUG
		printf("Failed to write/read I2C device address \n");
	#endif
		wq_enqueue(turn_off_light_sensor, NULL);
		record_sysview_measure_light_exit(1);
		return ;
	}
	*light->light = ((uint16_t)data_read[1] << 8) + data_read[0];
	*light->light = (uint32_t) (*light->light) * 82/100;
	DEBUG_SEGGER_SYSVIEW_PrintfHost("Light sensor: %d", *light->light);
#ifdef DEBUG
	printf("Light sensor: %d\n", *light->light);
#endif
	record_sysview_measure_light_exit(0);
	wq_enqueue(turn_off_light_sensor, NULL);
	return ;
}
