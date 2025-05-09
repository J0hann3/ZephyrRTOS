#include "sx126x.h"
#include "atmel_start_pins.h"
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>

#define CRITICAL_SECTION_BEGIN( ) uint32_t mask; BoardCriticalSectionBegin( &mask )
#define CRITICAL_SECTION_END( ) BoardCriticalSectionEnd( &mask )

static RadioOperatingModes_t OperatingMode;
static bool ImageCalibrated = false;
RadioOperatingModes_t SX126xGetOperatingMode(void) { return OperatingMode; }


void BoardCriticalSectionBegin(uint32_t* mask)
{
  *mask = __get_PRIMASK();
  __disable_irq();
}

void BoardCriticalSectionEnd(uint32_t* mask)
{
  __set_PRIMASK(*mask);
}

uint16_t SpiInOut(uint16_t outData)
{
  // Wait for bus idle (ready to write)
  while ((SERCOM_SPI_INTFLAG_DRE & hri_sercomspi_read_INTFLAG_reg(SERCOM4)) == 0)
  {
  }
  hri_sercomspi_clear_INTFLAG_reg(SERCOM4, SERCOM_SPI_INTFLAG_DRE);

  // Write byte
  hri_sercomspi_write_DATA_reg(SERCOM4, outData);

  // Wait for ready to read
  while ((SERCOM_SPI_INTFLAG_RXC & hri_sercomspi_read_INTFLAG_reg(SERCOM4)) == 0)
  {
  }
  hri_sercomspi_clear_INTFLAG_reg(SERCOM4, SERCOM_SPI_INTFLAG_RXC);

  // Read byte
  outData = (uint16_t)hri_sercomspi_read_DATA_reg(SERCOM4);

  return outData;
}

void SX126xWaitOnBusy(void)
{
  while (gpio_get_pin_level(SX126X_BUSY) == 1);
}

void SX126xSetOperatingMode(RadioOperatingModes_t mode)
{
  OperatingMode = mode;
#if defined(USE_RADIO_DEBUG)
  switch (mode)
  {
  case MODE_TX :
    SX126xDbgPinTxWrite(1);
    SX126xDbgPinRxWrite(0);
    break;
  case MODE_RX :
  case MODE_RX_DC :
    SX126xDbgPinTxWrite(0);
    SX126xDbgPinRxWrite(1);
    break;
  default :
    SX126xDbgPinTxWrite(0);
    SX126xDbgPinRxWrite(0);
    break;
  }
#endif
}

void SX126xWakeup(void)
{
  CRITICAL_SECTION_BEGIN();

  gpio_set_pin_level(SX126X_SS, 0);

  SpiInOut(RADIO_GET_STATUS);
  SpiInOut(0x00);

  gpio_set_pin_level(SX126X_SS, 1);

  // Wait for chip to be ready.
  SX126xWaitOnBusy();

  // Update operating mode context variable
  SX126xSetOperatingMode(MODE_STDBY_RC);

  CRITICAL_SECTION_END();
}

void SX126xAntSwOn(void) {}

void SX126xCheckDeviceReady(void)
{
  if ((SX126xGetOperatingMode() == MODE_SLEEP) || (SX126xGetOperatingMode() == MODE_RX_DC))
  {
    SX126xWakeup();
    // Switch is turned off when device is in sleep mode and turned on is all other modes
    SX126xAntSwOn();
  }
  SX126xWaitOnBusy();
}

void SX126xAntSwOff(void) {}

void SX126xWriteCommand(RadioCommands_t command, uint8_t *buffer,
  uint16_t size)
{
  SX126xCheckDeviceReady();

  gpio_set_pin_level(SX126X_SS, 0);

  SpiInOut((uint8_t)command);

  for (uint16_t i = 0; i < size; i++)
  {
    SpiInOut(buffer[i]);
  }

  gpio_set_pin_level(SX126X_SS, 1);

  if (command != RADIO_SET_SLEEP)
  {
    SX126xWaitOnBusy();
  }
}

void SX126xSetSleep(SleepParams_t sleepConfig)
{
  SX126xAntSwOff();

  uint8_t value = (((uint8_t)sleepConfig.Fields.WarmStart << 2) |
                   ((uint8_t)sleepConfig.Fields.Reset << 1) |
                   ((uint8_t)sleepConfig.Fields.WakeUpRTC));

  if (sleepConfig.Fields.WarmStart == 0)
  {
    // Force image calibration
    ImageCalibrated = false;
  }
  SX126xWriteCommand(RADIO_SET_SLEEP, &value, 1);
  SX126xSetOperatingMode(MODE_SLEEP);
}
