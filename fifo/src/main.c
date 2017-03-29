/*
 * Copyright (c) 2017 Chunghan Yi(chunghan.yi@gmail.com).
 *
 * FIFO example
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <misc/printk.h>
#include <offsets.h>

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

/* delay between greetings (in ms) */
#define SLEEPTIME 500

struct k_fifo my_fifo;

struct data_item_t {
	void *fifo_reserved;   /* 1st word reserved for use by fifo */
	uint32_t field1;
	uint32_t field2;
	uint32_t field3;
};

char __noinit __stack producer_stack_area[STACKSIZE];
char __noinit __stack consumer_stack_area[STACKSIZE];

void producer_thread(void *dummy1, void *dummy2, void *dummy3)
{
	struct data_item_t tx_data;
	int i = 1;
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);

	while (1) {
		tx_data.field1 = 10*i;
		tx_data.field2 = 20*i;
		tx_data.field3 = 30*i;

		/* send data to consumers */
		k_fifo_put(&my_fifo, &tx_data);

		k_sleep(SLEEPTIME);
		i++;
	}
}

void consumer_thread(void *dummy1, void *dummy2, void *dummy3)
{
	struct data_item_t  *rx_data;
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);

	while (1) {
		rx_data = k_fifo_get(&my_fifo, K_FOREVER);
		printk("(%s)rx_data->field1 : [%d]\n", __func__, rx_data->field1);
		printk("(%s)rx_data->field2 : [%d]\n", __func__, rx_data->field2);
		printk("(%s)rx_data->field3 : [%d]\n", __func__, rx_data->field3);

		k_sleep(SLEEPTIME);
	}
}

/* initialize a fifo statically */
//K_FIFO_DEFINE(my_fifo);
void main(void)
{
	/* initialize a fifo dynamically */
	k_fifo_init(&my_fifo);

	/* start a producer thread */
	k_thread_spawn(producer_stack_area, STACKSIZE, producer_thread, NULL, NULL, NULL,
			PRIORITY, 0, K_NO_WAIT);
	/* start a consumer thread */
	k_thread_spawn(consumer_stack_area, STACKSIZE, consumer_thread, NULL, NULL, NULL,
			PRIORITY, 0, K_NO_WAIT);
}
