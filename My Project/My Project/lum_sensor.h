#ifndef LUM_SENSOR_H
# define LUM_SENSOR_H

#include <hal_delay.h>
#include <driver_init.h>
#ifdef DEBUG
#include "printf.h"
#endif

typedef enum
{
    LIGHT_SENSOR_TURN_ON = 0,
    LIGHT_SENSOR_WRITE_COMMAND,
    LIGHT_SENSOR_READ_VALUE,
    LIGHT_SENSOR_TURN_OFF,
    LIGHT_SENSOR_ERROR,
    LIGHT_SENSOR_IDLE,
} e_light_state;

typedef struct s_light_measure
{
    struct io_descriptor *i2c_device;

    uint16_t *light;

    uint32_t time_start_measure;

    e_light_state state;
} light_measure;

# define SLAVE_ADDR_LUM		0x10

void measure_light_sensor();

#endif