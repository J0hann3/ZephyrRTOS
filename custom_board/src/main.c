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
#include <zephyr/drivers/i2c.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS_LED   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
// #define LED1_NODE DT_ALIAS(led1)
// #define LED2_NODE DT_ALIAS(led2)

#if !DT_NODE_HAS_STATUS_OKAY(LED0_NODE) // || !DT_NODE_HAS_STATUS_OKAY(LED1_NODE) || !DT_NODE_HAS_STATUS_OKAY(LED2_NODE)
#error "Unsupported board: led devicetree alias is not defined"
#endif

#define SW0_NODE DT_ALIAS(sw1)

#if !DT_NODE_HAS_STATUS_OKAY(SW0_NODE)
#error "Unsupported board: switch devicetree alias is not defined"
#endif

// #define PWM_LED0_NODE DT_ALIAS(pwm_led0)
// #define PWM_LED2_NODE DT_ALIAS(pwm_led2)

// #if !DT_NODE_HAS_STATUS_OKAY(PWM_LED0_NODE) || !DT_NODE_HAS_STATUS_OKAY(PWM_LED2_NODE)
// #error "Unsupported board: switch devicetree alias is not defined"
// #endif

#define GPIO_SPEC_INIT(node_id) GPIO_DT_SPEC_GET(node_id, gpios),
#define PWM_SPEC_INIT(node_id) PWM_DT_SPEC_GET(node_id),

static const struct gpio_dt_spec leds[] = {DT_FOREACH_CHILD(DT_PATH(leds), GPIO_SPEC_INIT)}; // Get all the leds from the device tree
#define LEDS_SIZE ARRAY_SIZE(leds)

// static struct pwm_dt_spec pwm_leds[] = {DT_FOREACH_CHILD(DT_NODELABEL(pwmleds), PWM_SPEC_INIT)};
// #define PWM_LEDS_SIZE ARRAY_SIZE(pwm_leds)

static const struct gpio_dt_spec sw0 = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

static struct gpio_callback button_callback; // struct for the interrupt when the switch is activated

void toggle_leds(const struct gpio_dt_spec led[], size_t size)
{
    static bool led_state = 0;
    int ret;

    for (size_t i = 0U; i < size; i++)
    {
        ret = gpio_pin_toggle_dt(&led[i]);
        if (ret < 0)
        return;
    }
    printf("Toggle led: %d\n", led_state);
    led_state =! led_state;
}

void toggle_leds_callback()
{
    toggle_leds(leds, LEDS_SIZE);
}

int init_button(const struct gpio_dt_spec button, struct gpio_callback *callback,
                gpio_callback_handler_t handler, bool interrupt)
{
    // Init Button to input with interrupt function in the struct callback
    int ret;

    if (!gpio_is_ready_dt(&button))
        return 1;

    ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (ret != 0)
        return 1;

    if (interrupt) 
    {
        printf("Init button interrupt\n");
        ret = gpio_pin_interrupt_configure_dt(&button,
            GPIO_INT_EDGE_BOTH); // trigger interrupt with rising edge
        if (ret != 0)
            return 1;
    
        gpio_init_callback(callback, handler, BIT(button.pin));
        return gpio_add_callback_dt(&button, callback);
    }
    return 0;
}

int init_leds(const struct gpio_dt_spec led[], size_t size)
{
    // Init all leds from the device tree to output
    int ret;

    for (size_t i = 0U; i < size; i++)
    {
        if (!gpio_is_ready_dt(&led[i])) {
            return 1;
        }
        ret = gpio_pin_configure_dt(&led[i], GPIO_OUTPUT_INACTIVE);
        if (ret < 0) {
            return 1;
        }
    }
    return 0;
}

int init_pwm_leds(struct pwm_dt_spec pwm_led[], size_t size)
{
    for (size_t i = 0U; i < size; i++)
    {   
        if (!pwm_is_ready_dt(&pwm_led[i]))
            return 1;
        if (pwm_led[i].channel == 2)
            pwm_led[i].flags = PWM_POLARITY_NORMAL;
        else
            pwm_led[i].flags = PWM_POLARITY_INVERTED;  
    }
    return 0;
}

void routine_led_pwm(struct pwm_dt_spec pwm_led[], size_t size)
{
    int period = 20000000;
    int pulse = 0;
    bool inc = true;

    while (1) {
        for (size_t i = 0U; i < size; i++)
        {
            pwm_set_dt(&pwm_led[i], period, pulse);
        }
        printf("Pulse : %d\n", pulse);
        k_msleep(SLEEP_TIME_MS_LED / 7);

        if (inc)
            pulse =  pulse + 500000;
        else
            pulse =  pulse - 500000;

        if (pulse <= 0 || pulse >= period)
            inc = !inc;
    } 
}

void routine_sw(const struct gpio_dt_spec led[], const struct gpio_dt_spec sw)
{
    static bool sw_state = false;

    if (gpio_pin_get(sw.port, sw.pin))
    {
        if (!sw_state)
        {
            printf("Press button\n");
            toggle_leds(led, LEDS_SIZE);
            sw_state = true;
            k_msleep(30);
        }
    }
    else
        sw_state = false;
}


int main()
{
    // if (init_pwm_leds(pwm_leds, PWM_LEDS_SIZE) == 1)
    //     return 1;
    if (init_leds(leds, LEDS_SIZE) == 1)
        return 1;

    if (init_button(sw0, &button_callback, toggle_leds_callback, 1) == 1)
        return 1;
    printf("Button configure\n");
    while (1) {
        // routine_sw(leds, sw0);
        // routine_led_pwm(pwm_leds, PWM_LEDS_SIZE);
        // toggle_leds(leds, LEDS_SIZE);
        // k_msleep(SLEEP_TIME_MS_LED);
    }
    return 0;
}
