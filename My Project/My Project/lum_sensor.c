#include "lum_sensor.h"
#include <SEGGER_SYSVIEW.h>

int init_lum_sensor(struct io_descriptor *lum_device)
{
	uint8_t   data_config[3] = {0x00, 0x00, 0x08};

	SEGGER_SYSVIEW_RecordU32x3(ID_SYSVIEW_E_CONFIG_LIGHT_SENSOR, data_config[0], data_config[1], data_config[2]);
	gpio_set_pin_direction(VCC_SENSORS_EN, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(VCC_SENSORS_EN, false);
	delay_ms(5);
	
	i2c_m_sync_get_io_descriptor(&I2C_0, &lum_device);
	i2c_m_sync_enable(&I2C_0);
	i2c_m_sync_set_slaveaddr(&I2C_0, SLAVE_ADDR_LUM, I2C_M_SEVEN);
	
	uint8_t check = io_write(lum_device, data_config, 3);
	SEGGER_SYSVIEW_RecordEndCallU32(ID_SYSVIEW_E_CONFIG_LIGHT_SENSOR, (check != 3) ? 1 : 0);
	if (check != 3)
		return 1;
	return 0;
}

uint8_t read_lum_sensor(uint16_t *light)
{
	uint8_t   data_read[2];
	
	SEGGER_SYSVIEW_RecordVoid(ID_SYSVIEW_E_MEASURE_LIGHT);
	i2c_m_sync_set_slaveaddr(&I2C_0, SLAVE_ADDR_LUM, I2C_M_SEVEN);

	if (i2c_m_sync_cmd_read(&I2C_0, 0x04, data_read, 2) != 0)
	{
	#ifdef DEBUG
		printf("Failed to write/read I2C device address \n");
	#endif
		SEGGER_SYSVIEW_RecordEndCallU32(ID_SYSVIEW_E_MEASURE_LIGHT, 1);
		return 1;
	}
	*light = ((uint16_t)data_read[1] << 8) + data_read[0];
	*light = (uint32_t) (*light) * 82/100;
	SEGGER_SYSVIEW_PrintfHost("Light sensor: %d", *light);
#ifdef DEBUG
	printf("Light sensor: %d\n", *light);
#endif
	SEGGER_SYSVIEW_RecordEndCallU32(ID_SYSVIEW_E_MEASURE_LIGHT, 0);
	return 0;
}

uint8_t init_and_read_lum_sensor(struct io_descriptor *lum_device, uint16_t *light)
{
	if (init_lum_sensor(lum_device) == 1){
	#ifdef DEBUG
		printf("Init i2c error\n");
	#endif
		return 1;
	}
	delay_ms(150);
	if (read_lum_sensor(light))
		return 1;
	
	gpio_set_pin_direction(VCC_SENSORS_EN, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(VCC_SENSORS_EN, GPIO_PULL_UP);
	return 0;
}
