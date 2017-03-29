/*
 * Copyright (c) 2017 Chunghan Yi(chunghan.yi@gmail.com).
 *
 * Mailbox example
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <stdio.h>
#include <errno.h>
#include <misc/printk.h>

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

/* delay between greetings (in ms) */
#define SLEEPTIME 2000

char __noinit __stack producer_stack_area[STACKSIZE];
char __noinit __stack consumer_stack_area[STACKSIZE];

struct k_mbox my_mailbox;

void producer_thread(void *dummy1, void *dummy2, void *dummy3)
{
	struct k_mbox_msg send_msg;

	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);

	while (1) {
		/* generate random value to send */
		uint32_t random_value = sys_rand32_get();

		/* prepare to send empty message */
		send_msg.info = random_value % 256;
		send_msg.size = 0;
		send_msg.tx_data = NULL;
		send_msg.tx_block.pool_id = NULL;
		send_msg.tx_target_thread = K_ANY;

		/* send message and wait until a consumer receives it */
		k_mbox_put(&my_mailbox, &send_msg, K_FOREVER);

		k_sleep(SLEEPTIME);
	}
}

void consumer_thread(void *dummy1, void *dummy2, void *dummy3)
{
	struct k_mbox_msg recv_msg;
	char buffer[100];
	int i;
	int total;

	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);

	while (1) {
		/* prepare to receive message */
		recv_msg.info = 100;
		recv_msg.size = 100;
		recv_msg.rx_source_thread = K_ANY;

		/* get a data item, waiting as long as needed */
		k_mbox_get(&my_mailbox, &recv_msg, buffer, K_FOREVER);

		/* info, size, and rx_source_thread fields have been updated */
		printk("(%s)recv_msg.info -> [%d]\n", __func__, recv_msg.info);

		/* verify that message data was fully received */
#if 0
		if (recv_msg.info != recv_msg.size) {
			printk("some message data dropped during transfer!\n");
			printk("sender tried to send %d bytes\n", recv_msg.info);
		}
#endif

		/* compute sum of all message bytes (from 0 to 100 of them) */
		total = 0;
		for (i = 0; i < recv_msg.size; i++) {
			total += buffer[i];
		}

		k_sleep(SLEEPTIME);
	}
}

void main(void)
{
	/* initialize a mailbox */
	k_mbox_init(&my_mailbox);

	/* start a producer thread */
	k_thread_spawn(producer_stack_area, STACKSIZE, producer_thread, NULL, NULL, NULL,
			PRIORITY, 0, K_NO_WAIT);
	/* start a consumer thread */
	k_thread_spawn(consumer_stack_area, STACKSIZE, consumer_thread, NULL, NULL, NULL,
			PRIORITY, 0, K_NO_WAIT);
}
