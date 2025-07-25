#ifndef LUM_SENSOR_H
# define LUM_SENSOR_H

#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>

#define I2C_LUM_NODE DT_NODELABEL(lum_sensor)

#if !DT_NODE_HAS_STATUS_OKAY(I2C_LUM_NODE)
#error "Unsupported board: sensor devicetree nodelabel is not defined"
#endif

int init_lum_sensor(const struct i2c_dt_spec *lum_device);
uint8_t read_lum_sensor(const struct i2c_dt_spec *lum_device, uint16_t *light);
uint8_t init_and_read_lum_sensor(const struct i2c_dt_spec *lum_device, uint16_t *light);

#endif