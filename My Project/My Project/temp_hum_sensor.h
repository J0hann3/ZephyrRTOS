#ifndef TEMP_HUM_SENSOR_H
# define TEMP_HUM_SENSOR_H

#include <hal_delay.h>
#include <driver_init.h>
#include <stdio.h>

# define SLAVE_ADDR_TEMP		0x44

typedef enum
{
    TEMP_SENSOR_WRITE_COMMAND = 0,
    TEMP_SENSOR_READ_VALUE,
    TEMP_SENSOR_ERROR,
    TEMP_SENSOR_IDLE,
} e_temp_state;

typedef struct s_temp_measure
{
    struct io_descriptor *i2c_device;

    uint16_t *temp;
    uint16_t *hum;

    uint32_t time_start_measure;

    e_temp_state state;
} temp_measure;

// uint8_t read_temp_sensor(temp_measure *);
void    hum_to_string(const uint16_t u16_hum, char s_hum[5]);
void    temp_to_string(const uint16_t u16_temp, char *s_temp);
void    ftoa(float fnum, char res[5]);

void measure_temp_hum_sensor();

#endif