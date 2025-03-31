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
#include <zephyr/drivers/pwm.h>

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

#define PWM_LED0_NODE DT_ALIAS(pwm_led0)
#define PWM_LED2_NODE DT_ALIAS(pwm_led2)

#if !DT_NODE_HAS_STATUS_OKAY(PWM_LED0_NODE) || !DT_NODE_HAS_STATUS_OKAY(PWM_LED2_NODE)
#error "Unsupported board: switch devicetree alias is not defined"
#endif

#define GPIO_SPEC_INIT(node_id) GPIO_DT_SPEC_GET(node_id, gpios),
#define PWM_SPEC_INIT(node_id) PWM_DT_SPEC_GET(node_id),

// static const struct gpio_dt_spec leds[] = {DT_FOREACH_CHILD(DT_NODELABEL(leds), GPIO_SPEC_INIT)}; // Get all the leds from the device tree

static struct pwm_dt_spec pwm_leds[] = {DT_FOREACH_CHILD(DT_NODELABEL(pwmleds), PWM_SPEC_INIT)};
// static const struct gpio_dt_spec sw0 = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);

// static struct gpio_callback button_callback; // struct for the interrupt when the switch is activated

// void toggle_leds()
// {
//     static bool led_state = 0;
//     int ret;

//     for (size_t i = 0U; i < ARRAY_SIZE(leds); i++)
//     {
//         printf("Toggle led: %d\n", led_state);
//         ret = gpio_pin_toggle_dt(&leds[i]);
//         if (ret < 0)
//             return;
//     }
//     led_state =! led_state;
// }

// int init_button()
// {
//     // Init Button to input with interrupt function in the struct callback
//     int ret;

//     if (!gpio_is_ready_dt(&sw0))
//         return 1;

//     ret = gpio_pin_configure_dt(&sw0, GPIO_INPUT);
//     if (ret != 0)
//         return 1;

//     ret = gpio_pin_interrupt_configure_dt(&sw0,
//     GPIO_INT_EDGE_RISING); // trigger interrupt with rising edge
//     if (ret != 0)
//         return 1;

//     gpio_init_callback(&button_callback, toggle_leds, BIT(sw0.pin));
//     gpio_add_callback_dt(&sw0, &button_callback);
//     return 0;
// }

// int init_leds()
// {
//     // Init all leds from the device tree to output
//     int ret;

//     for (size_t i = 0U; i < ARRAY_SIZE(leds); i++)
//     {
//         if (!gpio_is_ready_dt(&leds[i])) {
//             return 1;
//         }
//         ret = gpio_pin_configure_dt(&leds[i], GPIO_OUTPUT_INACTIVE);
//         if (ret < 0) {
//             return 1;
//         }
//     }
//     return 0;
// }

int init_pwm_leds()
{
    for (size_t i = 0U; i < ARRAY_SIZE(pwm_leds); i++)
    {   
        if (!pwm_is_ready_dt(&pwm_leds[i]))
            return 1;
        if (pwm_leds[i].channel == 2)
            pwm_leds[i].flags = PWM_POLARITY_NORMAL;
        else
            pwm_leds[i].flags = PWM_POLARITY_INVERTED;  
    }
    return 0;
}

int main()
{
    if (init_pwm_leds() == 1)
        return 1;
    // if (init_leds() == 1)
    //     return 1;
    // if (init_button() == 1)
    //     return 1;

    int period = 20000000;
    int pulse = 0;
    bool inc = true;

    while (1) {
        for (size_t i = 0U; i < ARRAY_SIZE(pwm_leds); i++)
        {
            pwm_set_dt(&pwm_leds[i], period, pulse);
        }
        printf("Pulse : %d\n", pulse);
        k_msleep(SLEEP_TIME_MS_LED / 7);

        if (inc)
            pulse =  pulse + 500000;
        else
            pulse =  pulse - 500000;

        if (pulse <= 0 || pulse >= period)
            inc = !inc;
        // for (size_t i = 0U; i < ARRAY_SIZE(leds); i++)
        // {
        //     ret = gpio_pin_toggle_dt(&leds[i]);
        //     if (ret < 0)
        //         return 1;
        // }
    }
}
