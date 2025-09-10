/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>
#include "timestamp.h"
#include "calendar.h"
#include "tools.h"
#include "i2c.h"

struct usart_sync_descriptor USART_0;

void USART_0_PORT_init(void)
{

	gpio_set_pin_function(PA08, PINMUX_PA08C_SERCOM0_PAD0);

	gpio_set_pin_function(PA09, PINMUX_PA09C_SERCOM0_PAD1);
}

void USART_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM0);
	_gclk_enable_channel(SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC);
}

void USART_0_init(void)
{
	USART_0_CLOCK_init();
	usart_sync_init(&USART_0, SERCOM0, (void *)NULL);
	USART_0_PORT_init();
	
	usart_sync_enable(&USART_0);
}

void delay_driver_init(void)
{
	delay_init(SysTick);
}

void system_init(void)
{
	init_mcu();

	_set_sleep_mode(3); // set power management to standby (Stop: CPU clock, AHB clock, APB clock, main clock; regulator low power; ram low power)

#ifdef DEBUG
	USART_0_init();
#endif

	I2C_0_init();

	SpiInit();

	delay_driver_init();

	CALENDAR_0_init();

#ifdef SEGGER_SYSTEM_VIEW
	TIMER_0_init();
#endif
}
