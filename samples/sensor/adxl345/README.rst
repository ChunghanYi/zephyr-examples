.. _adxl345:

ADXL345: 3-Axis Accelerometer
#######################################################

Overview
********

Requirements
************

References
**********

https://slowbootkernelhacks.blogspot.com/2024/11/zephyr-rtos-programming.html

Building and Running
********************

This project outputs sensor data to the console. It requires an ADXL345
system-in-package, which is present on the stm32 nucleo_l432kc board

.. zephyr-app-commands::
   :zephyr-app: samples/sensor/adxl345
   :board: stm32 nucleo_l432kc
   :goals: build
   :compact:

Sample Output
=============

.. code-block:: console

   Accelerometer data:
   ( x y z ) = ( 0.074221  -0.304696  0.972685 )

   <repeats endlessly every 2 seconds>
