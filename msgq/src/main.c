/*
 * Copyright (c) 2017 Chunghan Yi(chunghan.yi@gmail.com).
 *
 * Message queue example
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

struct data_item_type {
	uint32_t field1;
	uint32_t field2;
	uint32_t field3;
};

char __aligned(4) my_msgq_buffer[10 * sizeof(struct data_item_type)];
struct k_msgq my_msgq;

char __noinit __stack producer_stack_area[STACKSIZE];
char __noinit __stack consumer_stack_area[STACKSIZE];

void producer_thread(void *dummy1, void *dummy2, void *dummy3)
{
	struct data_item_type data;
	int i = 1;
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);

	while (1) {
		/* create data item to send (e.g. measurement, timestamp, ...) */
		data.field1 = 10*i;
		data.field2 = 20*i;
		data.field3 = 30*i;

		/* send data to consumers */
		while (k_msgq_put(&my_msgq, &data, K_NO_WAIT) != 0) {
			/* message queue is full: purge old data & try again */
			k_msgq_purge(&my_msgq);
		}

		/* data item was successfully added to message queue */
		//printk("(%s)data item was successfully added to message queue\n", __func__);

		k_sleep(SLEEPTIME);
		i++;
	}
}

void consumer_thread(void *dummy1, void *dummy2, void *dummy3)
{
	struct data_item_type data;
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);

	while (1) {
		/* get a data item */
		k_msgq_get(&my_msgq, &data, K_FOREVER);

		/* process data item */
		printk("(%s)data.field1 = [%d]\n", __func__, data.field1);
		printk("(%s)data.field2 = [%d]\n", __func__, data.field2);
		printk("(%s)data.field3 = [%d]\n", __func__, data.field3);

		k_sleep(SLEEPTIME);
	}
}

/* initialize message queue statically */
K_MSGQ_DEFINE(my_msgq, sizeof(struct data_item_type), 10, 4);

void main(void)
{
	/* initialize message queue dynamically */
#if 0
	k_msgq_init(&my_msgq, my_msgq_buffer, sizeof(struct data_item_type), 10);
#endif

	/* start a producer thread */
	k_thread_spawn(producer_stack_area, STACKSIZE, producer_thread, NULL, NULL, NULL,
			PRIORITY, 0, K_NO_WAIT);
	/* start a consumer thread */
	k_thread_spawn(consumer_stack_area, STACKSIZE, consumer_thread, NULL, NULL, NULL,
			PRIORITY, 0, K_NO_WAIT);
}
