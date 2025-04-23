// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <hal_delay.h>
#include <hal_gpio.h>

#include "app_common.h"
#include "atmel_start_pins.h"

void sd_card_set_CS_pin_as_input(void)
{
  gpio_set_pin_direction(SD_CS, GPIO_DIRECTION_IN);
  gpio_set_pin_pull_mode(SD_CS, GPIO_PULL_OFF);
}

void sd_card_set_CS_pin_as_lowpower(void)
{
  gpio_set_pin_direction(SD_CS, GPIO_DIRECTION_IN);
  gpio_set_pin_pull_mode(SD_CS, GPIO_PULL_DOWN);
}

void sd_card_set_CS_pin_as_output(void)
{
  gpio_set_pin_direction(SD_CS, GPIO_DIRECTION_OUT);
  gpio_set_pin_pull_mode(SD_CS, GPIO_PULL_OFF);
  gpio_set_pin_level(SD_CS, false);
}

void sd_card_power_on(void)
{
  gpio_set_pin_direction(VCC_SD_EN, GPIO_DIRECTION_OUT);
  gpio_set_pin_function(VCC_SD_EN, GPIO_PIN_FUNCTION_OFF);
  gpio_set_pin_level(VCC_SD_EN, false);
}

void sd_card_power_off(void)
{
  gpio_set_pin_direction(VCC_SD_EN, GPIO_DIRECTION_OUT);
  gpio_set_pin_function(VCC_SD_EN, GPIO_PIN_FUNCTION_OFF);
  gpio_set_pin_level(VCC_SD_EN, true);
}

bool sd_card_is_card_detected()
{
  bool SD_CARD_CS_pin_level = false;

  sd_card_set_CS_pin_as_input();

  delay_ms(1);

  SD_CARD_CS_pin_level = gpio_get_pin_level(SD_CS);

  sd_card_set_CS_pin_as_output();

  return SD_CARD_CS_pin_level;
}
/*******************************************************************************
 End of File
 */
