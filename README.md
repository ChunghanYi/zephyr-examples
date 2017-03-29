# Zephyr RTOS examples

	$ git clone https://gerrit.zephyrproject.org/r/zephyr zephyr-project
		=> Download zephyr OS codes
	$ cd zephyr-project
	$ git clone https://github.com/ChunghanYi/zephyr-examples
		=> Download zephyr-examples

	$ export ZEPHYR_GCC_VARIANT=zephyr
	$ export ZEPHYR_SDK_INSTALL_DIR={YOUR_PATH}/zephyr-sdk
		=> If you didn't install zephyr-sdk, download it at first from https://www.zephyrproject.org/downloads/tools
	$ source zephyr-env.sh

	<How to build zephyr-examples for Nucleo-F103RB board>
	$ cd zephyr-examples
	$ cd alert/
	$ make BOARD=nucleo_f103rb
		=> You can see the output files under the outdir/ directory.
	$ make BOARD=nucleo_f103rb flash
		=> You should install the openocd package at first.
	...

	For more information about the zephyr-project, see the following page please.
	https://www.zephyrproject.org/doc/getting_started/getting_started.html
		or
	http://slowbootkernelhacks.blogspot.kr/
