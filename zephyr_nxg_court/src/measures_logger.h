#ifndef MEASURES_LOGGER_H_
#define MEASURES_LOGGER_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * To reduce power consumption, the stack should be big enough to store all the measurements done in a day.
 * So only one copy to the SD card should be done every days.
 * As the worst case is a measurement every 5 minutes, that means (24*60min)/5min = 288 measurement.
 *
 * But the micro memory is not big enough to store that many measurements.
 * So it was decided to reduce the stack size by a factor of 2. And make a copy to SD card twice a day.
 */
#define MEASURES_LOGGER_MAX_SIZE 12

typedef enum
{
  /* Operation succeeded */
  MEASURES_LOGGER_RES_SUCCESS = 1,
  /* Operation failed */
  MEASURES_LOGGER_RES_FAILURE = 0

} MEASURES_LOGGER_RESULT;

typedef enum
{
  MEASURES_LOGGER_ERROR_OK = 0,
  MEASURES_LOGGER_INVALID_MEASURING_PERIOD,
  MEASURES_LOGGER_INVALID_PARAMETER,
  MEASURES_LOGGER_NOTHING_TO_READ,
  MEASURES_LOGGER_UNREADED_MEASURES,
  MEASURES_LOGGER_UNINITIALIZED
} MEASURES_LOGGER_ERROR;

typedef struct
{
  // struct calendar_date_time datetime;
  uint16_t                  temperature;
  uint16_t                  humidity;
  uint16_t                  brightness;

  // Sensors activation
  uint8_t TEMP_HUM_SENSOR_EN : 1;
  uint8_t LUM_SENSOR_EN      : 1;
} measure_t;

typedef struct
{
	uint16_t              head;  // write index
	uint16_t              tail;  // read index
	uint16_t              size;
	uint16_t              counter;
	MEASURES_LOGGER_ERROR errorValue;
	measure_t             measure[MEASURES_LOGGER_MAX_SIZE];
} measures_stack_t;

/**
 *  @brief Initialize stack size.
 *    Stack is sized to get two copy by day.
 *
 *  @param Period between 2 measurements in minutes
 *
 *  @retval   MEASURES_LOGGER_RES_SUCCESS
 *  @retval   MEASURES_LOGGER_RES_FAILURE
 */
bool measures_logger_init(const uint16_t measuring_period);

/**
 *  @brief Calculate difference between write and read index.
 *
 *  @param No parameters
 *
 *  @returns
 *      result of difference between write and read index.
 */
size_t measures_logger_count();

/**
 *  @brief
 *    Return stack size according to measuring_period given to
 *    measures_logger_init().
 *
 *    size = minutes in a day (1440) / measuring_period in minutes.
 *
 *  @param No parameters
 *
 *  @returns result stack size.
 */
uint16_t measures_logger_get_size();

/**
 *  @brief logger error.
 *
 *  @param No parameters
 *
 *  @returns
 *      last error.
 */
MEASURES_LOGGER_ERROR measures_logger_get_error();

/**
 *  @brief Write new_measure to m_stack at head index.
 *
 *  @param measure_t new_measure
 *
 *
 * @retval    MEASURES_LOGGER_RES_FAILURE if one of the parameters is NULL.
 * @retval    MEASURES_LOGGER_RES_FAILURE if overloop on unread data.
 * @retval    MEASURES_LOGGER_RES_SUCCESS.
 */
bool measures_logger_write(const measure_t* new_measure);

/**
 *  @brief Read last measure from m_stack at tail index.
 *
 *  @param measure_t last_measure will be updated
 *
 *  @retval   MEASURES_LOGGER_RES_SUCCESS
 *  @retval   MEASURES_LOGGER_RES_FAILURE
 */
bool measures_logger_read(measure_t* last_measure);

/**
 *  @brief Read last measure and return value as CSV line.
 *
 *  @param[in,out] char* csvLine
 *
 *  @retval   MEASURES_LOGGER_RES_SUCCESS
 *  @retval   MEASURES_LOGGER_RES_FAILURE
 */
bool measures_logger_read_CSV(char s_return[160]);

#endif /* MEASURES_LOGGER_H_ */