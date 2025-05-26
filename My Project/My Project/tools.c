#include "sx126x.h"
#include "atmel_start_pins.h"
#include <peripheral_clk_config.h>
#include <hal_spi_m_sync.h>
#include <hal_delay.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>
#include <hal_init.h>
#include <utils.h>

#define COMPARE_COUNT_MAX_VALUE (uint16_t)(-1)

struct spi_m_sync_descriptor Spi0;

void SpiInit()
{
  hri_gclk_write_CLKCTRL_reg(GCLK, GCLK_CLKCTRL_ID_SERCOM4_CORE | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_CLKEN);
  // pas de clock slow
  hri_pm_set_APBCMASK_SERCOM4_bit(PM);

  spi_m_sync_init(&Spi0, SERCOM4);

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

void SX126xIoInit(void)
{
  gpio_set_pin_direction(SX126X_SS, GPIO_DIRECTION_OUT);
  gpio_set_pin_pull_mode(SX126X_SS, GPIO_PULL_OFF);
  gpio_set_pin_level(SX126X_SS, true);

  gpio_set_pin_direction(SX126X_BUSY, GPIO_DIRECTION_IN);
  gpio_set_pin_pull_mode(SX126X_BUSY, GPIO_PULL_OFF);

  gpio_set_pin_direction(SX126X_DIO1, GPIO_DIRECTION_IN);
  gpio_set_pin_pull_mode(SX126X_DIO1, GPIO_PULL_OFF);
  gpio_set_pin_function(SX126X_DIO1, RADIO_INTERRUPT_PIN_PINMUX);
}

void SX126xIoDeInit(void)
{
  gpio_set_pin_direction(SX126X_SS, GPIO_DIRECTION_OUT);
  gpio_set_pin_pull_mode(SX126X_SS, GPIO_PULL_OFF);
  gpio_set_pin_level(SX126X_SS, 1);

  gpio_set_pin_direction(SX126X_BUSY, GPIO_DIRECTION_IN);
  gpio_set_pin_pull_mode(SX126X_BUSY, GPIO_PULL_OFF);

  gpio_set_pin_direction(SX126X_DIO1, GPIO_DIRECTION_IN);
  gpio_set_pin_pull_mode(SX126X_DIO1, GPIO_PULL_OFF);
}

void spi_go_to_sleep()
{
  SpiInit();
  // Wakeup
  SX126xIoInit();

  gpio_set_pin_function(SX126X_SS, GPIO_PIN_FUNCTION_OFF);
  gpio_set_pin_direction(SX126X_SS, GPIO_DIRECTION_OUT);
  gpio_set_pin_level(SX126X_SS, false);
  // Wait for busy
  delay_ms(1);

  SleepParams_t sleep_params;

  sleep_params.Fields.WarmStart = 1;
  sleep_params.Fields.WakeUpRTC = 0;

  SX126xSetSleep(sleep_params);
  // Wait for 1ms to ensure all the configuration is retained
  delay_ms(1);

  SpiDeInit();

  // Set pin direction LORA ChipSelect
  gpio_set_pin_direction(SX126X_SS, GPIO_DIRECTION_IN);
  gpio_set_pin_pull_mode(SX126X_SS, GPIO_PULL_UP);
  
  SX126xIoDeInit();
}