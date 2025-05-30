#include "spi.h"

int spi_sam0_init(const struct device *dev);

void spi_deinit()
{
    const struct device *porta = DEVICE_DT_GET(DT_ALIAS(port_a));
    const struct device *portb = DEVICE_DT_GET(DT_ALIAS(port_b));

    // SPI
    gpio_pin_configure(portb, 11, GPIO_INPUT | GPIO_PULL_DOWN);     // miso
    gpio_pin_configure(porta, 12, GPIO_INPUT | GPIO_PULL_DOWN);     // mosi
    gpio_pin_configure(porta, 13, GPIO_INPUT | GPIO_PULL_DOWN);     // sck
}

void spi_init()
{
    spi_sam0_init(DEVICE_DT_GET(DT_NODELABEL(sercom4)));
    k_msleep(1);
}