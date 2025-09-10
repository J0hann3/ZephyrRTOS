#ifndef LUM_SENSOR_H
# define LUM_SENSOR_H

#include <hal_delay.h>
#include <driver_init.h>
#ifdef DEBUG
#include "printf.h"
#endif

typedef struct s_light_measure
{
    struct io_descriptor *i2c_device;

    uint16_t *light;

    uint32_t time_start_measure;
} light_measure;

# define SLAVE_ADDR_LUM		0x10

void turn_on_light_sensor(void *const arg);
void turn_off_light_sensor();
void light_sensor_write_command(void *const arg);
void read_lum_sensor(void *const arg);

#endif