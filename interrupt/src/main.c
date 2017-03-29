/*
 * Copyright (c) 2017 Chunghan Yi(chunghan.yi@gmail.com).
 *
 * Interrupt example
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <misc/printk.h>
#include <offsets.h>
#include <string.h>

#define MY_DEV_IRQ  24       /* device uses IRQ 24 */
#define MY_DEV_PRIO  2       /* device uses interrupt priority 2 */
/* argument passed to my_isr(), in this case a pointer to the device */
#define MY_ISR_ARG  DEVICE_GET(my_device)
#define MY_IRQ_FLAGS 0       /* IRQ flags. Unused on non-x86 */

void my_isr(void *arg)
{
	/* ISR code */
}

void my_isr_installer(void)
{
	IRQ_CONNECT(MY_DEV_IRQ, MY_DEV_PRIO, my_isr, MY_ISR_ARG, MY_IRQ_FLAGS);
	irq_enable(MY_DEV_IRQ);
}

void main(void)
{
	my_isr_installer();
}
