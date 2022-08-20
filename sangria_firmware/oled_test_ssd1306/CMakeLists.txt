if (TARGET tinyusb_device)
	add_executable(
		oled_test_ssd1306
		main.cpp
		sangria_firmware.cpp
		sangria_oled.cpp
	)

	# pull in common dependencies
	target_link_libraries(oled_test_ssd1306 pico_stdlib hardware_i2c)

	# enable usb output, disable uart output
	pico_enable_stdio_usb(oled_test_ssd1306 1)
	pico_enable_stdio_uart(oled_test_ssd1306 0)

	# create map/bin/hex file etc.
	pico_add_extra_outputs(oled_test_ssd1306)
elseif(PICO_ON_DEVICE)
	message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()
