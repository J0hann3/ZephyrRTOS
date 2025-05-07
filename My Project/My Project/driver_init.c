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

struct spi_m_sync_descriptor SPI_0;

struct usart_sync_descriptor USART_0;

struct i2c_m_sync_desc I2C_0;

// struct calendar_descriptor CALENDAR_0;

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

void I2C_0_PORT_init(void)
{

	gpio_set_pin_pull_mode(PA16,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA16, PINMUX_PA16C_SERCOM1_PAD0);

	gpio_set_pin_pull_mode(PA17,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA17, PINMUX_PA17C_SERCOM1_PAD1);
}

void I2C_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM1);
	_gclk_enable_channel(SERCOM1_GCLK_ID_CORE, CONF_GCLK_SERCOM1_CORE_SRC);
	_gclk_enable_channel(SERCOM1_GCLK_ID_SLOW, CONF_GCLK_SERCOM1_SLOW_SRC);
}

void I2C_0_init(void)
{
	I2C_0_CLOCK_init();
	i2c_m_sync_init(&I2C_0, SERCOM1);
	I2C_0_PORT_init();
}

//void SPI_0_PORT_init(void)
//{
//
	//gpio_set_pin_level(PA12,
	                   //// <y> Initial level
	                   //// <id> pad_initial_level
	                   //// <false"> Low
	                   //// <true"> High
	                   //false);
//
	//// Set pin direction to output
	//gpio_set_pin_direction(PA12, GPIO_DIRECTION_OUT);
//
	//gpio_set_pin_function(PA12, PINMUX_PA12D_SERCOM4_PAD0);
//
	//gpio_set_pin_level(PA13,
	                   //// <y> Initial level
	                   //// <id> pad_initial_level
	                   //// <false"> Low
	                   //// <true"> High
	                   //false);
//
	//// Set pin direction to output
	//gpio_set_pin_direction(PA13, GPIO_DIRECTION_OUT);
//
	//gpio_set_pin_function(PA13, PINMUX_PA13D_SERCOM4_PAD1);
//
	//// Set pin direction to input
	//gpio_set_pin_direction(PB11, GPIO_DIRECTION_IN);
//
	//gpio_set_pin_pull_mode(PB11,
	                       //// <y> Pull configuration
	                       //// <id> pad_pull_config
	                       //// <GPIO_PULL_OFF"> Off
	                       //// <GPIO_PULL_UP"> Pull-up
	                       //// <GPIO_PULL_DOWN"> Pull-down
	                       //GPIO_PULL_OFF);
//
	//gpio_set_pin_function(PB11, PINMUX_PB11D_SERCOM4_PAD3);
//}
//
//void SPI_0_CLOCK_init(void)
//{
	//_pm_enable_bus_clock(PM_BUS_APBC, SERCOM4);
	//_gclk_enable_channel(SERCOM4_GCLK_ID_CORE, CONF_GCLK_SERCOM4_CORE_SRC);
//}

void SPI_0_init(void)
{
	hri_gclk_write_CLKCTRL_reg(GCLK, GCLK_CLKCTRL_ID_SERCOM4_CORE | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_CLKEN);
	// pas de clock slow
	hri_pm_set_APBCMASK_SERCOM4_bit(PM);

	spi_m_sync_init(&SPI_0, SERCOM4);

	hri_sercomspi_wait_for_sync(SERCOM4, SERCOM_SPI_SYNCBUSY_SWRST);
	hri_sercomspi_set_CTRLA_SWRST_bit(SERCOM4);
	hri_sercomspi_wait_for_sync(SERCOM4, SERCOM_SPI_SYNCBUSY_SWRST);

	hri_sercomspi_write_CTRLA_reg(SERCOM4, SERCOM_SPI_CTRLA_MODE(3) | SERCOM_SPI_CTRLA_DOPO(0) | SERCOM_SPI_CTRLA_DIPO(3));

	hri_sercomspi_write_CTRLB_reg(SERCOM4, SERCOM_SPI_CTRLB_RXEN);
	hri_sercomspi_write_BAUD_reg(SERCOM4, ((float)CONF_GCLK_SERCOM4_CORE_FREQUENCY / (float)(2 * 1000000)) - 1);
	hri_sercomspi_write_DBGCTRL_reg(SERCOM4, 0);

	// Set pin direction to input. MISO
	gpio_set_pin_direction(SPI_MISO, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(SPI_MISO, GPIO_PULL_OFF);
	gpio_set_pin_function(SPI_MISO, SPI_MISO_PINMUX);

	// Set pin direction to output. MOSI
	gpio_set_pin_direction(SPI_MOSI, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(SPI_MOSI, false);
	gpio_set_pin_function(SPI_MOSI, SPI_MOSI_PINMUX);

	// Set pin direction to output. CLK
	gpio_set_pin_direction(SPI_SCK, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(SPI_SCK, false);
	gpio_set_pin_function(SPI_SCK, SPI_SCK_PINMUX);

	hri_sercomspi_set_CTRLA_ENABLE_bit(SERCOM4);
}

void SpiDeInit()
{
	// NSS will remain high, no deinit

	// #define U32_DRV_GPIO_SPI_MISO_LORA Pin
	gpio_set_pin_direction(SPI_MISO, GPIO_DIRECTION_IN);
	gpio_set_pin_function(SPI_MISO, GPIO_PIN_FUNCTION_OFF);
	gpio_set_pin_pull_mode(SPI_MISO, GPIO_PULL_DOWN);

	// #define U32_DRV_GPIO_SPI_MOSI_LORA Pin
	gpio_set_pin_direction(SPI_MOSI, GPIO_DIRECTION_IN);
	gpio_set_pin_function(SPI_MOSI, GPIO_PIN_FUNCTION_OFF);
	gpio_set_pin_pull_mode(SPI_MOSI, GPIO_PULL_DOWN);

	// #define U32_DRV_GPIO_SPI_SCK_LORA Pin
	gpio_set_pin_direction(SPI_SCK, GPIO_DIRECTION_IN);
	gpio_set_pin_function(SPI_SCK, GPIO_PIN_FUNCTION_OFF);
	gpio_set_pin_pull_mode(SPI_SCK, GPIO_PULL_DOWN);
}

void system_init(void)
{
	init_mcu();

	_set_sleep_mode(3); // set power management to standby (Stop: CPU clock, AHB clock, APB clock, main clock; regulator low power; ram low power)

#ifdef DEBUG
	USART_0_init();
#endif

	I2C_0_init();

	SPI_0_init();

	CALENDAR_0_init();
}
