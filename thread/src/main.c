/*
 * Copyright (c) 2017 Chunghan Yi(chunghan.yi@gmail.com).
 *
 * thread example
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <misc/printk.h>
#include <offsets.h>
#include <string.h>

/* size of stack area used by each thread */
#define MY_STACK_SIZE 500

/* scheduling priority used by each thread */
#define MY_PRIORITY 5

/* delay between greetings (in ms) */
#define SLEEPTIME 500

char __noinit __stack my_stack_area[MY_STACK_SIZE];

void my_entry_point(void *dummy1, void *dummy2, void *dummy3)
{
	int count = 0;

    ARG_UNUSED(dummy1);
    ARG_UNUSED(dummy2);
    ARG_UNUSED(dummy3);

	while (1) {
		count++;
		printk("(%s) count ==> %d\n", __func__, count);
		if (count > 50) {
			printk("(%s) thread is quit.\n", __func__);
			break;
		}

		k_sleep(SLEEPTIME);
	}
}

void main(void)
{
	/* start a thread dynamically */
#if 0
	k_tid_t my_tid = k_thread_spawn(my_stack_area, MY_STACK_SIZE, my_entry_point, NULL, NULL, NULL,
			MY_PRIORITY, 0, K_NO_WAIT);
#endif
}

/* start a thread statically */
K_THREAD_DEFINE(my_tid, MY_STACK_SIZE,
				my_entry_point, NULL, NULL, NULL,
				MY_PRIORITY, 0, K_NO_WAIT);
