#include "lum_sensor.h"
#include "timestamp.h"
#include "i2c.h"
#include <SEGGER_SYSVIEW.h>
#include "work_queue.h"

#define DELAY_LIGHT_SENSOR_TURN_ON 5 // in ms
#define DELAY_LIGHT_SENSOR_MEASURE 150 // in ms

static void turn_on_light_sensor(light_measure *light);
static void turn_off_light_sensor(light_measure *light);
static void light_sensor_write_command(light_measure *light);
static void read_lum_sensor(light_measure *light);

void measure_light_sensor(void *const arg)
{
	light_measure *light = arg;

	switch(light->state)
	{
		case LIGHT_SENSOR_TURN_ON:
			turn_on_light_sensor(light);
			break;
		case LIGHT_SENSOR_WRITE_COMMAND:
			light_sensor_write_command(light);
			break;
		case LIGHT_SENSOR_READ_VALUE:
			read_lum_sensor(light);
			break;
		case LIGHT_SENSOR_TURN_OFF:
		case LIGHT_SENSOR_ERROR:
			turn_off_light_sensor(light);
			break;
		case LIGHT_SENSOR_IDLE:
			break;
		default:
			break;
	}
	if (light->state != LIGHT_SENSOR_IDLE)
		wq_enqueue(measure_light_sensor, arg);
	else
		light->state = LIGHT_SENSOR_TURN_ON;
}

static void turn_on_light_sensor(light_measure *light)
{
	gpio_set_pin_direction(VCC_SENSORS_EN, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(VCC_SENSORS_EN, false);
	light->time_start_measure = get_timestamp();
	light->state = LIGHT_SENSOR_WRITE_COMMAND;
}

static void turn_off_light_sensor(light_measure *light)
{
	gpio_set_pin_direction(VCC_SENSORS_EN, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(VCC_SENSORS_EN, GPIO_PULL_UP);
	light->state = LIGHT_SENSOR_IDLE;
}

static void light_sensor_write_command(light_measure *light)
{
	uint8_t   data_config[3] = {0x00, 0x00, 0x08};

	if (!is_delay_reach(DELAY_LIGHT_SENSOR_TURN_ON, light->time_start_measure))
		return ;

	i2c_m_sync_get_io_descriptor(&I2C_0, &light->i2c_device);
	i2c_m_sync_enable(&I2C_0);
	i2c_m_sync_set_slaveaddr(&I2C_0, SLAVE_ADDR_LUM, I2C_M_SEVEN);
	
	uint8_t check = io_write(light->i2c_device, data_config, 3);
	if (check != 3)
	{
		light->state = LIGHT_SENSOR_ERROR;
	#ifdef DEBUG
		printf("Init i2c error\n");
	#endif
		return;
	}
	light->time_start_measure = get_timestamp();
	light->state = LIGHT_SENSOR_READ_VALUE;
}

static void read_lum_sensor(light_measure *light)
{
	uint8_t   data_read[2];

	if (!is_delay_reach(DELAY_LIGHT_SENSOR_MEASURE, light->time_start_measure))
		return ;
	
	record_sysview_measure_light_enter();
	i2c_m_sync_set_slaveaddr(&I2C_0, SLAVE_ADDR_LUM, I2C_M_SEVEN);

	if (i2c_m_sync_cmd_read(&I2C_0, 0x04, data_read, 2) != 0)
	{
	#ifdef DEBUG
		printf("Failed to write/read I2C device address \n");
	#endif
		light->state = LIGHT_SENSOR_ERROR;
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
	light->state = LIGHT_SENSOR_TURN_OFF;
	return ;
}
