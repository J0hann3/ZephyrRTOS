#ifndef TEMP_HUM_SENSOR_H
# define TEMP_HUM_SENSOR_H

#include <zephyr/drivers/i2c.h>

#define I2C_TEMP_NODE DT_NODELABEL(temp_sensor)

#if !DT_NODE_HAS_STATUS_OKAY(I2C_TEMP_NODE)
#error "Unsupported board: sensor devicetree nodelabel is not defined"
#endif

uint8_t read_temp_sensor(const struct i2c_dt_spec *temp_device, uint16_t *temp, uint16_t *humidity);
int routine_read_temp(const struct i2c_dt_spec *temp_device);


#endif