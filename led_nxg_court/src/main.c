/*
* Copyright (c) 2016 Intel Corporation
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/util.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS_LED   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)

#if !DT_NODE_HAS_STATUS_OKAY(LED0_NODE) || !DT_NODE_HAS_STATUS_OKAY(LED1_NODE) || !DT_NODE_HAS_STATUS_OKAY(LED2_NODE)
#error "Unsupported board: led devicetree alias is not defined"
#endif

#define SW0_NODE DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS_OKAY(SW0_NODE)
#error "Unsupported board: switch devicetree alias is not defined"
#endif

#define GPIO_SPEC_INIT(node_id) GPIO_DT_SPEC_GET(node_id, gpios),

static const struct gpio_dt_spec leds[] = {DT_FOREACH_CHILD(DT_NODELABEL(leds), GPIO_SPEC_INIT)}; // Get all the leds from the device tree

static const struct gpio_dt_spec sw0 = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);

static struct gpio_callback button_callback; // struct for the interrupt when the switch is activated

void toggle_led()
{
    int ret;

    for (size_t i = 0U; i < ARRAY_SIZE(leds); i++)
    {
        ret = gpio_pin_toggle_dt(&leds[i]);
        if (ret < 0)
            return;
    }
}

int main()
{
    int ret;

    // Init all leds from the device tree to output
    for (size_t i = 0U; i < ARRAY_SIZE(leds); i++)
    {
        if (!gpio_is_ready_dt(&leds[i])) {
            return 1;
        }
        ret = gpio_pin_configure_dt(&leds[i], GPIO_OUTPUT_INACTIVE);
        if (ret < 0) {
            return 1;
        }
    }

    // Init Button to input with interrupt function in the struct callback
    if (!gpio_is_ready_dt(&sw0))
        return 1;

    ret = gpio_pin_configure_dt(&sw0, GPIO_INPUT);
    if (ret != 0)
        return 1;

    ret = gpio_pin_interrupt_configure_dt(&sw0,
        GPIO_INT_EDGE_RISING); // trigger interrupt with rising edge
    if (ret != 0)
        return 0;

    gpio_init_callback(&button_callback, toggle_led, BIT(sw0.pin));
    gpio_add_callback_dt(&sw0, &button_callback);

    // while (1) {
    //     for (size_t i = 0U; i < ARRAY_SIZE(leds); i++)
    //     {
    //         ret = gpio_pin_toggle_dt(&leds[i]);
    //         if (ret < 0)
    //             return 1;
    //     }
    //     k_msleep(SLEEP_TIME_MS_LED);
    // }
    return 0;
}
