#include <stdio.h>
#include <string.h>

#include "measures_logger.h"
#include "temp_hum_sensor.h"

measures_stack_t m_stack;

bool measures_logger_init(const uint16_t measuring_period)
{
  uint16_t measuring_periode_min = 1;
  uint16_t measuring_periode_max = 5;

  m_stack.errorValue             = MEASURES_LOGGER_ERROR_OK;

  if (measuring_period >= measuring_periode_min &&
      measuring_period <= measuring_periode_max)  // Input value filtering
  {
    /* stack is sized to get two copy by day. */
    m_stack.size = (MEASURES_LOGGER_MAX_SIZE * measuring_periode_min) / measuring_period;
  }
  else
  {
    m_stack.size       = MEASURES_LOGGER_MAX_SIZE;
    m_stack.errorValue = MEASURES_LOGGER_INVALID_MEASURING_PERIOD;
    return MEASURES_LOGGER_RES_FAILURE;
  }
  return MEASURES_LOGGER_RES_SUCCESS;
}

size_t measures_logger_count()
{
  return m_stack.counter;
}

uint16_t measures_logger_get_size()
{
  return m_stack.size;
}

MEASURES_LOGGER_ERROR measures_logger_get_error()
{
  // _ASSERT(m_stack.errorValue == MEASURES_LOGGER_ERROR_OK);
  return m_stack.errorValue;
}

/*
 *  Summary:
 *    Read last measure from m_stack at tail index.
 */
bool measures_logger_read(measure_t *last_measure)
{
  if (m_stack.size == 0)
  {
	  m_stack.errorValue = MEASURES_LOGGER_UNINITIALIZED;
	  return MEASURES_LOGGER_RES_FAILURE;
  }
	
  if (last_measure == NULL)
  {
    m_stack.errorValue = MEASURES_LOGGER_INVALID_PARAMETER;
    return MEASURES_LOGGER_RES_FAILURE;
  }
  if (m_stack.counter <= 0)
  {
    m_stack.errorValue = MEASURES_LOGGER_NOTHING_TO_READ;
    return MEASURES_LOGGER_RES_FAILURE;
  }
  
  // _ASSERT(m_stack.tail < MEASURES_LOGGER_MAX_SIZE);

  *last_measure = m_stack.measure[m_stack.tail];

  /* Reset measure */
  // m_stack.measure[m_stack.tail].datetime.date.year  = 0;
  // m_stack.measure[m_stack.tail].datetime.date.month = 0;
  // m_stack.measure[m_stack.tail].datetime.date.day   = 0;
  // m_stack.measure[m_stack.tail].datetime.time.hour  = 0;
  // m_stack.measure[m_stack.tail].datetime.time.min   = 0;
  // m_stack.measure[m_stack.tail].datetime.time.sec   = 0;
  m_stack.measure[m_stack.tail].temperature         = 0;
  m_stack.measure[m_stack.tail].humidity            = 0;
  m_stack.measure[m_stack.tail].brightness          = 0;

  /* Incrementing the index */
  m_stack.tail = (m_stack.tail + 1) % m_stack.size;
  /* Decrement counter of element in the stack */
  m_stack.counter--;

  m_stack.errorValue = MEASURES_LOGGER_ERROR_OK;
  return MEASURES_LOGGER_RES_SUCCESS;
}

/*
 *  Summary:
 *    Write new_measure to m_stack at head index.
 */
bool measures_logger_write(const measure_t *new_measure)
{
  if (m_stack.size == 0)
  {
	  m_stack.errorValue = MEASURES_LOGGER_UNINITIALIZED;
	  return MEASURES_LOGGER_RES_FAILURE;
  }
  if (new_measure == NULL)
  {
    m_stack.errorValue = MEASURES_LOGGER_INVALID_PARAMETER;
    return MEASURES_LOGGER_RES_FAILURE;
  }
  
  // _ASSERT(m_stack.head < MEASURES_LOGGER_MAX_SIZE);

  if (m_stack.counter != 0 && ((m_stack.head) % m_stack.size) == m_stack.tail)
  {
    /*Don't write on unread measure*/
    m_stack.errorValue = MEASURES_LOGGER_UNREADED_MEASURES;
    return MEASURES_LOGGER_RES_FAILURE;
  }

  m_stack.measure[m_stack.head] = *new_measure;
  m_stack.head                  = (m_stack.head + 1) % m_stack.size;
  m_stack.counter               = m_stack.counter + 1;
  m_stack.errorValue            = MEASURES_LOGGER_ERROR_OK;
  return MEASURES_LOGGER_RES_SUCCESS;
}

/*
 * Read last measure and return value as CSV line.
 */
bool measures_logger_read_CSV(char s_return[160])
{
  measure_t         measureTo_Write;
  const char        s_desactivated[]     = "DESACTIVATED";
  char              s_measure[160]       = {'\0'};
  char              s_temperature[13]    = {'\0'};
  char              s_humidity[13]       = {'\0'};
  char              s_brightness[13]     = {'\0'};

  if (s_return == NULL)
  {
    m_stack.errorValue = MEASURES_LOGGER_INVALID_PARAMETER;
    return MEASURES_LOGGER_RES_FAILURE;
  }

  bool ml_read_error = measures_logger_read(&measureTo_Write);
  if (ml_read_error == MEASURES_LOGGER_RES_FAILURE)
  {
    return MEASURES_LOGGER_RES_FAILURE;
  }

  // Temperature and humidity control
  if (measureTo_Write.TEMP_HUM_SENSOR_EN)
  {
    temp_to_string(measureTo_Write.temperature, s_temperature);
    hum_to_string(measureTo_Write.humidity, s_humidity);
  }
  else
  {
    strncpy(s_temperature, s_desactivated, sizeof(s_desactivated));
    strncpy(s_humidity, s_desactivated, sizeof(s_desactivated));
  }

  // Brightness control
  if (measureTo_Write.TEMP_HUM_SENSOR_EN)
    snprintf(s_brightness, 6, "%05u", measureTo_Write.brightness);
  else
    strncpy(s_brightness, s_desactivated, sizeof(s_desactivated));

  snprintf(s_measure, 21, "%04u-%02u-%02uT%02u:%02u:%02u;",
           0,
           0,
           0,
           0,
           0,
           0);

  strncat(s_measure, s_temperature, strlen(s_temperature));
  strcat(s_measure, ";");

  strncat(s_measure, s_humidity, strlen(s_humidity));
  strcat(s_measure, ";");

  strncat(s_measure, s_brightness, strlen(s_brightness));

  strncpy(s_return, s_measure, strlen(s_measure));

  return MEASURES_LOGGER_RES_SUCCESS;
}
