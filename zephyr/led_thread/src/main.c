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
#define SLEEP_TIME_MS_LED1   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)

#if !DT_NODE_HAS_STATUS_OKAY(LED0_NODE)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS_OKAY(LED1_NODE)
#error "Unsupported board: led1 devicetree alias is not defined"
#endif

#define MY_STACK_SIZE 500
#define MY_PRIORITY 5

/*
* A build error on this line means your board is unsupported.
* See the sample documentation for information on how to fix this.
*/
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);

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
        printf("LED state %d: %s\n",led->pin, led_state ? "ON" : "OFF");
        if (led->pin == 2)
            k_msleep(SLEEP_TIME_MS_LED0);
        else
            k_msleep(SLEEP_TIME_MS_LED1);
    }
}

K_THREAD_DEFINE(blink0_id, MY_STACK_SIZE, blink_led, &led0, NULL, NULL,
    MY_PRIORITY, 0, 0);

K_THREAD_DEFINE(blink1_id, MY_STACK_SIZE, blink_led, &led1, NULL, NULL,
	MY_PRIORITY, 0, 0);