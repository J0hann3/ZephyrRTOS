#ifndef LUM_SENSOR_H
# define LUM_SENSOR_H

#include <hal_delay.h>
#include <driver_init.h>
#include "printf.h"

# define SLAVE_ADDR_LUM		0x10

int init_lum_sensor(struct io_descriptor *lum_device);
uint8_t read_lum_sensor(struct io_descriptor *lum_device, uint16_t *light);
uint8_t init_and_read_lum_sensor(struct io_descriptor *lum_device, uint16_t *light);

#endif