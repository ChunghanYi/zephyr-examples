/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2017 Chunghan Yi(chunghan.yi@gmail.com).
 *
 * Alert example
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <misc/printk.h>
#include <offsets.h>
#include <string.h>

#include <board.h>
#include <device.h>
#include <gpio.h>
#include <misc/util.h>

/* change this to use another GPIO port */
#ifdef SW0_GPIO_NAME
#define PORT	SW0_GPIO_NAME
#else
#error SW0_GPIO_NAME needs to be set in board.h
#endif

/* change this to use another GPIO pin */
#ifdef SW0_GPIO_PIN
#define PIN     SW0_GPIO_PIN
#else
#error SW0_GPIO_PIN needs to be set in board.h
#endif

/* change to use another GPIO pin interrupt config */
#ifdef SW0_GPIO_INT_CONF
#define EDGE    SW0_GPIO_INT_CONF
#else
/*
 * If SW0_GPIO_INT_CONF not defined used default EDGE value.
 * Change this to use a different interrupt trigger
 */
#define EDGE    (GPIO_INT_EDGE | GPIO_INT_ACTIVE_LOW)
#endif

/* change this to enable pull-up/pull-down */
#define PULL_UP 0

/* Sleep time */
#define SLEEP_TIME	500

/* for thread related codes */
#define MY_STACK_SIZE (K_THREAD_SIZEOF + 500)
#define MY_PRIORITY 5
char __noinit __stack my_stack_area[MY_STACK_SIZE];

struct k_alert my_alert;

int my_alert_handler(struct k_alert *alert_id_is_unused)
{
	printk("(%s) called.\n", __func__);
#if 0
	return 0; /* alert has been consumed */
#else
	return 1; /* alert should be pended */
#endif
}

void my_thread(void *dummy1, void *dummy2, void *dummy3)
{
    ARG_UNUSED(dummy1);
    ARG_UNUSED(dummy2);
    ARG_UNUSED(dummy3);

    while (1) {
		/* wait for a key press alert to pend */
        k_alert_recv(&my_alert, K_FOREVER);

		printk("(%s)after k_alert_recv() ...\n", __func__);
    }
}

void button_pressed(struct device *gpiob, struct gpio_callback *cb,
		    uint32_t pins)
{
	printk("(%s)Button pressed at %d\n", __func__, k_cycle_get_32());

	k_alert_send(&my_alert);
}

static struct gpio_callback gpio_cb;

void do_gpio_interrupt(void)
{
	struct device *gpiob;

	printk("Press the user defined button on the board\n");
	gpiob = device_get_binding(PORT);
	if (!gpiob) {
		printk("error\n");
		return;
	}

	gpio_pin_configure(gpiob, PIN,
			   GPIO_DIR_IN | GPIO_INT |  PULL_UP | EDGE);

	gpio_init_callback(&gpio_cb, button_pressed, BIT(PIN));

	gpio_add_callback(gpiob, &gpio_cb);
	gpio_pin_enable_callback(gpiob, PIN);

	while (1) {
		uint32_t val = 0;

		gpio_pin_read(gpiob, PIN, &val);
		k_sleep(SLEEP_TIME);
	}
}

void main(void)
{
	/* initialize an alert */
	k_alert_init(&my_alert, my_alert_handler, 10);

	/* start a thread */
	k_thread_spawn(my_stack_area, MY_STACK_SIZE, my_thread, NULL, NULL, NULL,
			MY_PRIORITY, 0, K_NO_WAIT);

	/* register GPIO interrupt handler */
	do_gpio_interrupt();
}
