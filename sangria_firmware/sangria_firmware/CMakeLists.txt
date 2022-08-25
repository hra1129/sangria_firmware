if (TARGET tinyusb_device)
	add_executable(
		sangria_firmware
		main.cpp
		sangria_firmware.cpp
		sangria_jogdial.cpp
		sangria_keyboard.cpp
		sangria_oled.cpp
		sangria_logo.cpp
		# sangria_usb_keyboard.cpp
	)

	# pull in common dependencies
	target_link_libraries(sangria_firmware pico_stdlib hardware_i2c)

	# enable usb output, disable uart output
	pico_enable_stdio_usb(sangria_firmware 1)
	pico_enable_stdio_uart(sangria_firmware 0)

	# create map/bin/hex file etc.
	pico_add_extra_outputs(sangria_firmware)
elseif(PICO_ON_DEVICE)
	message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()
