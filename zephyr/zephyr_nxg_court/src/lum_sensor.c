#include "lum_sensor.h"

int init_lum_sensor(const struct i2c_dt_spec *lum_device, const struct gpio_dt_spec *vcc_sensor)
{
    // Init Vcc sensor to then configure i2c device(light sensor)
    if (gpio_pin_configure_dt(vcc_sensor, GPIO_OUTPUT_ACTIVE) < 0) 
        return 1;

    k_msleep(5);
    
    uint8_t config[3] = {0x00, 0x00, 0x08}; // { commmand config, config LSB, config MSB }, config => gain 1, IT => 100ms, Pers => 1

    if (i2c_write_dt(lum_device, config, 3) != 0)
        return 1;
    return 0;
}

uint8_t read_lum_sensor(const struct i2c_dt_spec *lum_device, uint16_t *light)
{
    uint8_t sensor_reg = 0x04;
    uint8_t lum_reading[2]= {0};

    if(i2c_write_read_dt(lum_device, &sensor_reg, 1, lum_reading, 2) != 0){
    #ifdef DEBUG
        printf("Failed to write/read I2C device address \n");
    #endif
        return 1;
    }
    *light = ((uint16_t)lum_reading[1] << 8) + lum_reading[0];
    *light = (uint32_t) (*light) * 82/100;
#ifdef DEBUG
    printf("Light sensor: %d\n", *light);
#endif
    return 0;
}

uint8_t init_and_read_lum_sensor(const struct i2c_dt_spec *lum_device, const struct gpio_dt_spec *vcc_sensor, uint16_t *light)
{
	if (init_lum_sensor(lum_device, vcc_sensor) == 1){
    #ifdef DEBUG
		printf("Init i2c error\n");
    #endif
		return 1;
	}
    k_msleep(150);
	if (read_lum_sensor(lum_device, light))
		return 1;
	
    gpio_pin_configure_dt(vcc_sensor, GPIO_INPUT | GPIO_PULL_UP);
	return 0;
}
