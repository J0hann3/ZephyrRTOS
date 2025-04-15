#include "lum_sensor.h"

int init_lum_sensor(const struct i2c_dt_spec *lum_device, const struct gpio_dt_spec *vcc_sensor)
{
    // Init Vcc sensor to then configure i2c device(light sensor)
    if (gpio_pin_configure_dt(vcc_sensor, GPIO_OUTPUT_ACTIVE) < 0) 
        return 1;

    k_msleep(5);
    
    uint8_t config[3] = {0x00, 0x00, 0x00}; // { commmand config, config LSB, config MSB }, config => gain 1, IT => 100ms, Pers => 1

    if (i2c_write_dt(lum_device, config, 3) != 0)
        return 1;
    return 0;
}

uint8_t read_lum_sensor(const struct i2c_dt_spec *lum_device, uint16_t *light)
{
    uint8_t sensor_reg = 0x04;
    uint8_t lum_reading[2]= {0};

    int ret = i2c_write_read_dt(lum_device, &sensor_reg, 1, &lum_reading[0], 2);
    if(ret != 0){
        printf("Failed to write/read I2C device address %x at Reg. %x \n\r", lum_device->addr, sensor_reg);
        return 1;
    }
    *light = ((uint16_t)lum_reading[1] << 8) + lum_reading[0];
    printf("Light sensor: %d\n", *light);
    return 0;
}

uint8_t init_and_read_lum_sensor(const struct i2c_dt_spec *lum_device, const struct gpio_dt_spec *vcc_sensor, uint16_t *light)
{
	if (init_lum_sensor(lum_device, vcc_sensor) == 1){
		printf("Init i2c error\n");
		return 1;
	}
    k_msleep(150);
	if (read_lum_sensor(lum_device, light))
		return 1;
	
    gpio_pin_configure_dt(vcc_sensor, GPIO_INPUT | GPIO_PULL_UP);
	return 0;
}

int routine_read_lum(const struct i2c_dt_spec *lum_device)
{
    uint16_t light;
    while (1)
    {
        k_msleep(1000);
        if (read_lum_sensor(lum_device, &light) == 1)
            return 1;
    }
    return 0;
}
