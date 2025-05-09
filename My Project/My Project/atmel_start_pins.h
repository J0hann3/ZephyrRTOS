/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAMD21 has 8 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7

#define PA08 GPIO(GPIO_PORTA, 8)
#define PA09 GPIO(GPIO_PORTA, 9)
#define PA12 GPIO(GPIO_PORTA, 12)
#define PA13 GPIO(GPIO_PORTA, 13)
#define PA16 GPIO(GPIO_PORTA, 16)
#define PA17 GPIO(GPIO_PORTA, 17)
#define PB11 GPIO(GPIO_PORTB, 11)

#define VCC_SENSORS_EN	GPIO(GPIO_PORTA, 25)
#define VCC_SD_EN		GPIO(GPIO_PORTB, 02)

#define SPI_MISO            GPIO(GPIO_PORTB, 11)
#define SD_CS				GPIO(GPIO_PORTB, 22)
#define SPI_MOSI            GPIO(GPIO_PORTA, 12)
#define SPI_SCK             GPIO(GPIO_PORTA, 13)

#define SPI_MISO_PINMUX     PINMUX_PB11D_SERCOM4_PAD3
#define SPI_MOSI_PINMUX     PINMUX_PA12D_SERCOM4_PAD0
#define SPI_SCK_PINMUX      PINMUX_PA13D_SERCOM4_PAD1

#define RADIO_INTERRUPT_PIN_PINMUX PINMUX_PA15A_EIC_EXTINT15

#define MAGNET_DETECTOR_VCC GPIO(GPIO_PORTA, 2)
#define NFC_FD              GPIO(GPIO_PORTA, 3)
#define MAGNET_DETECTOR_OUT GPIO(GPIO_PORTA, 4)
#define PUSH_REVERSE_BUTTON GPIO(GPIO_PORTA, 5)
#define VCC_MICRO_EN        GPIO(GPIO_PORTA, 6)
#define MIC_PDM_DATA        GPIO(GPIO_PORTA, 7)
#define MCU_DBG_UART_TX     GPIO(GPIO_PORTA, 8)
#define MCU_DBG_UART_RX     GPIO(GPIO_PORTA, 9)
#define MIC_PDM_CLK         GPIO(GPIO_PORTA, 10)
#define NFC_VCC             GPIO(GPIO_PORTA, 11)
#define ORANGE_LED_CMD      GPIO(GPIO_PORTA, 14)
#define SX126X_DIO1         GPIO(GPIO_PORTA, 15)
#define SDA                 GPIO(GPIO_PORTA, 16)
#define SCL                 GPIO(GPIO_PORTA, 17)
#define GREEN_LED_CMD       GPIO(GPIO_PORTA, 18)
#define RED_LED_CMD         GPIO(GPIO_PORTA, 19)
#define SX126X_BUSY         GPIO(GPIO_PORTA, 20)
#define SX126X_NRESET       GPIO(GPIO_PORTA, 21)
#define CONFIG_PIR_TX       GPIO(GPIO_PORTA, 22)
#define DATA_PIR_RX         GPIO(GPIO_PORTA, 23)
#define GREEN_REV_LED_CMD   GPIO(GPIO_PORTA, 24)
#define RED_REV_LED_CMD     GPIO(GPIO_PORTA, 27)
#define CMD_BUZZER          GPIO(GPIO_PORTA, 28)
#define ZMOD_RST            GPIO(GPIO_PORTB, 3)
#define VCC_ADC_CMD         GPIO(GPIO_PORTB, 8)
#define VCC_3V3_ADC         GPIO(GPIO_PORTB, 9)
#define SX126X_SS           GPIO(GPIO_PORTB, 10)
#define PUSH_BUTTON         GPIO(GPIO_PORTB, 23)

#endif // ATMEL_START_PINS_H_INCLUDED
