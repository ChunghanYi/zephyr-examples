/*
 * Copyright (c) 2017 Chunghan Yi(chunghan.yi@gmail.com).
 *
 * Stack example
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <misc/printk.h>
#include <offsets.h>
#include <string.h>

/* size of stack area used by each thread */
#define THREAD_STACKSIZE 1024

/* scheduling priority used by each thread */
#define THREAD_PRIORITY 7

/* delay between greetings (in ms) */
#define SLEEPTIME 2000

/* stack size */
#define MAX_ITEMS 10

uint32_t my_stack_array[MAX_ITEMS];
struct k_stack my_stack;

char __noinit __stack thread_stack_area[THREAD_STACKSIZE];

void thread_func(void *dummy1, void *dummy2, void *dummy3)
{
	uint32_t my_buffers[MAX_ITEMS];
	uint32_t *new_buffer;

	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);

	while (1) {
		/* save address of each data structure in a stack */
		for (int i = 0; i < MAX_ITEMS; i++) {
			my_buffers[i] = 10*i;
			k_stack_push(&my_stack, (uint32_t)&my_buffers[i]);
			printk("(%s) pushed data(=%d) to stack.\n", __func__, my_buffers[i]);
		}
		k_sleep(SLEEPTIME);

		for (int i = 0; i < MAX_ITEMS; i++) {
			k_stack_pop(&my_stack, (uint32_t *)&new_buffer, K_FOREVER);
			printk("(%s)popped data => [%d]\n", __func__, *new_buffer);
		}
		printk("========================================================\n");
		k_sleep(SLEEPTIME);
	}
}

/* initialize a stack statically */
//K_STACK_DEFINE(my_stack, MAX_ITEMS);

void main(void)
{
	/* initialize a stack dynamically */
	k_stack_init(&my_stack, my_stack_array, MAX_ITEMS);
	printk("k_stack_init() called.\n");

	/* start a producer thread */
	k_thread_spawn(thread_stack_area, THREAD_STACKSIZE, thread_func, NULL, NULL, NULL,
			THREAD_PRIORITY, 0, K_NO_WAIT);
}
