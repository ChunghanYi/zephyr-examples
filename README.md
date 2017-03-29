# Zephyr RTOS examples

	$ git clone https://github.com/ChunghanYi/zephyr-examples

	$ git clone https://gerrit.zephyrproject.org/r/zephyr zephyr-project
	$ cd zephyr-project
	$ cp -r ../zephyr-examples .

	$ export ZEPHYR_GCC_VARIANT=zephyr
	$ export ZEPHYR_SDK_INSTALL_DIR={YOUR_PATH}/zephyr-sdk
	$ source zephyr-env.sh

	<How to build zephyr-examples for Nucleo-F103RB board>
	$ cd zephyr-examples
	$ cd alert
	$ make BOARD=nucleo_f103rb
	$ make BOARD=nucleo_f103rb flash
	...

	For more information about the zephyr-project, see the following page please.
	https://www.zephyrproject.org/doc/getting_started/getting_started.html
		or
	http://slowbootkernelhacks.blogspot.kr/
