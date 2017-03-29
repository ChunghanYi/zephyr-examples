/*
 * Copyright (c) 2017 Chunghan Yi(chunghan.yi@gmail.com).
 *
 * Timer example
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <misc/printk.h>
#include <offsets.h>
#include <string.h>

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

/* delay between greetings (in ms) */
#define SLEEPTIME 500

struct k_timer my_timer;
extern void my_work_handler(struct k_work *work);
struct k_work my_work = K_WORK_INITIALIZER(my_work_handler);
char __noinit __stack thread_stack_area[STACKSIZE];

void my_work_handler(struct k_work *work)
{
    /* do the processing that needs to be done periodically */
	printk("(%s) called.\n", __func__);
}

void my_timer_function(struct k_timer *dummy)
{
	k_work_submit(&my_work);
}

void thread_func(void *dummy1, void *dummy2, void *dummy3)
{
	int count = 0;

    ARG_UNUSED(dummy1);
    ARG_UNUSED(dummy2);
    ARG_UNUSED(dummy3);

	while (1) {
		if (k_timer_status_get(&my_timer) > 0) {
			/* timer has expired */
			printk("(%s) timer has expired.\n", __func__);
		} else if (k_timer_remaining_get(&my_timer) == 0) {
			/* timer was stopped (by someone else) before expiring */
			printk("(%s) timer was stopped (by someone else) before expiring\n", __func__);
		} else {
			/* timer is still running */
			printk("(%s) timer is still running\n", __func__);
		}

		k_sleep(SLEEPTIME);

		count++;
		printk("(%s) count ==> %d\n", __func__, count);
		if (count > 50) {
			k_timer_stop(&my_timer);
			printk("(%s) thread is quit.\n", __func__);
			break;
		}
	}
}

void main(void)
{
	/* init a timer dynamically */
	k_timer_init(&my_timer, my_timer_function, NULL);

	/* start periodic timer that expires once every second */
	k_timer_start(&my_timer, K_SECONDS(1), K_SECONDS(1));
	
	/* start a thread */
	k_thread_spawn(thread_stack_area, STACKSIZE, thread_func, NULL, NULL, NULL,
			PRIORITY, 0, K_NO_WAIT);
}
