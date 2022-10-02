cmake_minimum_required(VERSION 3.13)

add_executable( r3_rp2040_fw )

target_sources( r3_rp2040_fw PUBLIC
	controller.cpp
	battery_level.cpp
	custom_menu.cpp
	main.cpp
)

# Make sure TinyUSB can find tusb_config.h
target_include_directories( r3_rp2040_fw PUBLIC ${CMAKE_CURRENT_LIST_DIR} ../rp2040_drivers )

# pull in common dependencies
target_link_libraries(r3_rp2040_fw PUBLIC rp2040_drivers pico_stdlib pico_multicore hardware_i2c hardware_adc hardware_gpio tinyusb_device tinyusb_board)

# enable usb output, disable uart output
# pico_enable_stdio_usb(r3_rp2040_fw 1)
# pico_enable_stdio_uart(r3_rp2040_fw 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(r3_rp2040_fw)
