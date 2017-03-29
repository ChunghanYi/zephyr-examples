/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2017 Chunghan Yi(chunghan.yi@gmail.com).
 *
 * Work queue example
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

/* for workqueue related codes */
#define MY_STACK_SIZE (K_THREAD_SIZEOF + 500)
#define MY_PRIORITY 5

char __noinit __stack my_stack_area[MY_STACK_SIZE];
struct k_work_q my_work_q;	/* work queue */
struct device_info {
	struct k_work work;		/* work */
	char name[16];
} my_device;


void button_pressed(struct device *gpiob, struct gpio_callback *cb,
		    uint32_t pins)
{
	printk("Button pressed at %d\n", k_cycle_get_32());

	/* do deferring work */
	k_work_submit(&my_device.work);	/* submit a work */
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

/* work handler */
void work_func(struct k_work *item)
{
	struct device_info *the_device =
		CONTAINER_OF(item, struct device_info, work);
	printk("Executing work_func on device %s\n", the_device->name);
}

void main(void)
{
	/* create a workqueue start */
	k_work_q_start(&my_work_q, my_stack_area, MY_STACK_SIZE, MY_PRIORITY);

	/* initialize name info for a device */
	strcpy(my_device.name, "FOO_dev");

	/* initialize work item for printing device's error messages */
	k_work_init(&my_device.work, work_func);

	/* register interrupt handler */
	do_gpio_interrupt();
}
