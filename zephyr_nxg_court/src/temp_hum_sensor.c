#include "temp_hum_sensor.h"

uint8_t read_temp_sensor(const struct i2c_dt_spec *temp_device, uint16_t *temp, uint16_t *humidity)
{
    uint8_t sensor_reg = 0xFD;
    uint8_t reading[6]= {0};

    // function i2c_write_read_dt doesn't generate a stop condition before switching to a read, no supported by all i2c sensor
    if(i2c_write_dt(temp_device, &sensor_reg, 1) != 0){
        printf("Failed to write/read I2C device address %x at Reg. %x \n\r", temp_device->addr, sensor_reg);
        return 1;
    }
    k_msleep(10);
    i2c_read_dt(temp_device, reading, 6);
    (*temp) = reading[0] * 256 + reading[1];
    (*temp) = -45 + 175 * (*temp) / 65535;

    (*humidity) = reading[3] * 256 + reading[4];
    (*humidity) = -6 + 125 * (*humidity) / 65535;
    printf("Temperature: %d, Humidity %d\n", (*temp), (*humidity));
    return 0;
}


int routine_read_temp(const struct i2c_dt_spec *temp_device)
{
    uint16_t temp;
    uint16_t humidity;

    while (1)
    {
        k_msleep(1000);
        read_temp_sensor(temp_device, &temp, &humidity);
    }
    return 0;
}