#include "temp_hum_sensor.h"
#include "i2c.h"
#include "work_queue.h"
#include <SEGGER_SYSVIEW.h>

void temp_sensor_read_value(void *const temp_hum);

void temp_sensor_write_command(void *const arg)
{
	temp_measure *temp_hum = arg;
	uint8_t sensor_reg = 0xFD;

	if (get_i2c_is_busy())
	{
		wq_enqueue(temp_sensor_write_command, temp_hum);
		return;
	}
	set_i2c_is_busy(true);
	record_sysview_measure_temp_enter();
	i2c_m_sync_get_io_descriptor(&I2C_0, &temp_hum->i2c_device);
	i2c_m_sync_enable(&I2C_0);
	i2c_m_sync_set_slaveaddr(&I2C_0, SLAVE_ADDR_TEMP, I2C_M_SEVEN);
	uint8_t check = io_write(temp_hum->i2c_device, &sensor_reg, 1);
	if (check != 1)
	{
		set_i2c_is_busy(false);
	#ifdef DEBUG
		printf("Failed to write/read I2C device address\n");
	#endif
		record_sysview_measure_temp_exit(1);
		return;
	}
	wq_enqueue(temp_sensor_read_value, temp_hum);
}

void temp_sensor_read_value(void *const arg)
{
	// add delay
	delay_ms(10);
	temp_measure *temp_hum = arg;
	uint8_t reading[6]= {0};
	uint32_t u32_temp;
	uint32_t u32_hum;

	uint8_t check = io_read(temp_hum->i2c_device, reading, 6);
	set_i2c_is_busy(false);
	if (check != 6)
	{
	#ifdef DEBUG
		printf("Error invalid number of bytes read: %d\n", check);
	#endif
		record_sysview_measure_temp_exit(1);
		return ;
	}

	//Add 100�C to temp and temp multiple by 10 to keep float precision
	temp_hum->temp = (reading[0] << 8) + reading[1];
	u32_temp = (uint32_t)((temp_hum->temp * 267 / 10000) + 1000);
	u32_temp = u32_temp - 450;
	temp_hum->temp = (uint16_t)u32_temp;

	//Humidity multiple by 10 to keep float precision
	temp_hum->hum = (reading[3] << 8) + reading[4];
	u32_hum = ((uint32_t)(temp_hum->hum * 1907) - 6000000) / 100000;
	temp_hum->hum = (uint16_t)u32_hum;

	SEGGER_SYSVIEW_PrintfHost("Temperature: %d, Humidity %d\n", (temp_hum->temp - 1000) / 10, temp_hum->hum/ 10);

	SEGGER_SYSVIEW_DATA_SAMPLE TempPlot;
	U32 temp = (temp_hum->temp - 1000);
	TempPlot.ID = 0;
	TempPlot.pValue.pU32 = &temp;
	SEGGER_SYSVIEW_SampleData(&TempPlot);

	SEGGER_SYSVIEW_DATA_SAMPLE HumPlot;
	U32 hum = temp_hum->hum;
	HumPlot.ID = 1;
	HumPlot.pValue.pU32 = &hum;
	SEGGER_SYSVIEW_SampleData(&HumPlot);
#ifdef DEBUG
	printf("Temperature: %d, Humidity %d\n", ((*temp) - 1000) / 10, (*humidity)/ 10);
#endif
	record_sysview_measure_temp_exit(0);
}

// void read_temp_sensor(temp_measure *temp_hum)
// {
// 	temp_sensor_write_command(temp_hum);
// 	delay_ms(10);
// 	temp_sensor_read_value(temp_hum);
// }

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