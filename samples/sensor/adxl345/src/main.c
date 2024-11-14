/*
 * Copyright (c) 2024 Chunghan Yi <chunghan.yi@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdlib.h>

#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

static const struct device *adxl345 = DEVICE_DT_GET(DT_NODELABEL(adxl345));

int main(void)
{
	static struct sensor_value accel_x, accel_y, accel_z;

	if (!device_is_ready(adxl345)) {
		printk("The ADXL345 sensor is not ready.");
		return 0;
	}
	printk("OK, sensor device is ready.\n");

	while (1) {
		sensor_sample_fetch_chan(adxl345, SENSOR_CHAN_ACCEL_XYZ);
		sensor_channel_get(adxl345, SENSOR_CHAN_ACCEL_X, &accel_x);
		sensor_channel_get(adxl345, SENSOR_CHAN_ACCEL_Y, &accel_y);
		sensor_channel_get(adxl345, SENSOR_CHAN_ACCEL_Z, &accel_z);

		printk("[val1] x: %d, y: %d, z: %d\n", accel_x.val1, accel_y.val1, accel_z.val1);
		printk("[val2] x: %d, y: %d, z: %d\n", accel_x.val2, accel_y.val2, accel_z.val2);

		k_msleep(2000);
	}
	return 0;
}
