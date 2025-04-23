#ifndef TEMP_HUM_SENSOR_H
# define TEMP_HUM_SENSOR_H

#include <hal_delay.h>
#include <driver_init.h>
#include <stdio.h>

# define SLAVE_ADDR_TEMP		0x44

uint8_t read_temp_sensor(struct io_descriptor *temp_device, uint16_t *temp, uint16_t *humidity);
void    hum_to_string(const uint16_t u16_hum, char s_hum[5]);
void    temp_to_string(const uint16_t u16_temp, char *s_temp);
void    ftoa(float fnum, char res[5]);

#endif