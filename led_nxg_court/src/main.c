/*
* Copyright (c) 2016 Intel Corporation
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS_LED0   500

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

#if !DT_NODE_HAS_STATUS_OKAY(LED0_NODE)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

void blink_led(const struct gpio_dt_spec *led)
{
    int ret;
    bool led_state = true;

    if (!gpio_is_ready_dt(led)) {
        return;
    }

    ret = gpio_pin_configure_dt(led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        return;
    }

    while (1) {
        ret = gpio_pin_toggle_dt(led);
        if (ret < 0) {
            return;
        }

        led_state = !led_state;
        printf("LED state: %s\n", led_state ? "ON" : "OFF");
        k_msleep(SLEEP_TIME_MS_LED0);
    }
}

int main()
{
    blink_led(&led0);
}
