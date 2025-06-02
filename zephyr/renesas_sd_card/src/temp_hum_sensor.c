#include "temp_hum_sensor.h"

uint8_t read_temp_sensor(const struct i2c_dt_spec *temp_device, uint16_t *temp, uint16_t *humidity)
{
    uint8_t sensor_reg = 0xFD;
    uint8_t reading[6]= {0};
    uint32_t u32_temp;
	  uint32_t u32_humidity;

    // function i2c_write_read_dt doesn't generate a stop condition before switching to a read, no supported by all i2c sensor
    if(i2c_write_dt(temp_device, &sensor_reg, 1) != 0){
    #ifdef DEBUG
      printf("Failed to write/read I2C device address\n");
    #endif
      return 1;
    }
    k_msleep(10);
    if (i2c_read_dt(temp_device, reading, 6) != 0)
    {
    #ifdef DEBUG
      printf("Error invalid number of bytes read\n");
    #endif
      return 1;
    }

    //Add 100�C to temp and temp multiple by 10 to keep float precision
    (*temp) = (reading[0] << 8) + reading[1];
    u32_temp = (uint32_t)(((*temp) * 267 / 10000) + 1000);
    u32_temp = u32_temp - 450;
    (*temp) = (uint16_t)u32_temp;
    //(*temp) = -45 + 175 * (*temp) / 65535;

    //Humidity multiple by 10 to keep float precision
    (*humidity) = (reading[3] << 8) + reading[4];
    u32_humidity = ((uint32_t)((*humidity) * 1907) - 6000000) / 100000;
    (*humidity) = (uint16_t)u32_humidity;
    //(*humidity) = -6 + 125 * (*humidity) / 65535;

  #ifdef DEBUG
    printf("Temperature: %d, Humidity %d\n", ((*temp) - 1000) / 10, (*humidity)/ 10);
  #endif
    return 0;
}

void ftoa(float fnum, char res[5])
{
  float tmpVal  = (fnum < 0) ? -fnum : fnum;

  int   tmpInt1 = tmpVal;            // Get the integer
  float tmpFrac = tmpVal - tmpInt1;  // Get fraction
  int   tmpInt2 = (tmpFrac * 100);   // Turn into integer

  snprintf(res, 5, "%02d.%01d", tmpInt1, tmpInt2);
}

/**
 * @brief convert temperature from u16 to string readable format : +AA.A
 */
void temp_to_string(const uint16_t u16_temp, char *s_temp)
{
  float f_temperature;
  char  s_temperature[5];
  char *tmpSign;

  f_temperature = ((float)(u16_temp - 1000) / 10);
  ftoa(f_temperature, s_temperature);

  tmpSign = (f_temperature < 0) ? "-" : "+";

  snprintf(s_temp, 6, "%s%s", tmpSign, s_temperature);
}

/**
 * @brief convert humidity from u16 to string readable format : AA.A
 */
void hum_to_string(const uint16_t u16_hum, char s_hum[5])
{
  float f_humidity;

  f_humidity = ((float)u16_hum) / 10;
  ftoa(f_humidity, s_hum);
}