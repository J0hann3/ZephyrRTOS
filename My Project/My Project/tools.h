/*
 * tools.h
 *
 * Created: 15/04/2025 15:20:34
 *  Author: JohanneVIGNY
 */ 


#ifndef TOOLS_H_
#define TOOLS_H_

#include "printf.h"

typedef enum
{
	PERIPHERAL_STATUS_OK            = 0,
	PERIPHERAL_STATUS_NOK           = 1,
	PERIPHERAL_STATUS_NOT_CONNECTED = 2,
	PERIPHERAL_STATUS_DISABLE       = 3,
	PERIPHERAL_END_OF_LIFE          = 4
} PeripheralStatus;

typedef struct
{
	PeripheralStatus temp_hum_sensor;
	PeripheralStatus co2_sensor;
	PeripheralStatus cov_sensor;
	PeripheralStatus pir_sensor;
	PeripheralStatus lum_sensor;
	PeripheralStatus micro;
	PeripheralStatus magnet_detector;
	PeripheralStatus sd_card;
	bool             low_battery;
	bool             pre_alarm_low_bat;
} DiagProductStatus;

bool diag_get_current_product_status(DiagProductStatus *ps);

/**
 * @brief Get the product minimal measuring periode
 *
 * @return uint16_t Minimal measuring periode
 */
uint16_t get_measuring_periode_min(void);

/**
 * @brief Get the product maximal measuring periode
 *
 * @return uint16_t Maximal measuring periode
 */
uint16_t get_measuring_periode_max(void);

void ftoa(float fnum, char res[5]);


#endif /* TOOLS_H_ */