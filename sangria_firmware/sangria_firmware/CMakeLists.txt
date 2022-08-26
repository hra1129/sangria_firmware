cmake_minimum_required(VERSION 3.13)

add_executable( sangria_firmware )

target_sources( sangria_firmware PUBLIC
	main.cpp
	sangria_firmware.cpp
	sangria_jogdial.cpp
	sangria_keyboard.cpp
	sangria_oled.cpp
	sangria_logo.cpp
	sangria_usb_keyboard.cpp
	usb_descriptors.c
)

# Make sure TinyUSB can find tusb_config.h
target_include_directories( sangria_firmware PUBLIC ${CMAKE_CURRENT_LIST_DIR} )

# pull in common dependencies
target_link_libraries(sangria_firmware PUBLIC pico_stdlib pico_multicore hardware_i2c tinyusb_device tinyusb_board)

# enable usb output, disable uart output
# pico_enable_stdio_usb(sangria_firmware 1)
# pico_enable_stdio_uart(sangria_firmware 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(sangria_firmware)
