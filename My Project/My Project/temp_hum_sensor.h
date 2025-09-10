#ifndef TEMP_HUM_SENSOR_H
# define TEMP_HUM_SENSOR_H

#include <hal_delay.h>
#include <driver_init.h>
#include <stdio.h>

# define SLAVE_ADDR_TEMP		0x44

typedef struct s_temp_measure
{
    struct io_descriptor *i2c_device;

    uint16_t temp;
    uint16_t hum;

    uint32_t time_start_measure;
} temp_measure;

// uint8_t read_temp_sensor(temp_measure *);
void    hum_to_string(const uint16_t u16_hum, char s_hum[5]);
void    temp_to_string(const uint16_t u16_temp, char *s_temp);
void    ftoa(float fnum, char res[5]);

// For work queue
void temp_sensor_write_command(void *const);
void temp_sensor_read_value(void *const);

#endif