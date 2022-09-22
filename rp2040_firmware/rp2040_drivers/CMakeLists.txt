cmake_minimum_required(VERSION 3.13)

pico_add_impl_library( rp2040_drivers )

target_sources( rp2040_drivers INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/sangria_jogdial.cpp
	${CMAKE_CURRENT_LIST_DIR}/sangria_keyboard.cpp
	${CMAKE_CURRENT_LIST_DIR}/sangria_i2c.cpp
	${CMAKE_CURRENT_LIST_DIR}/sangria_oled.cpp
	${CMAKE_CURRENT_LIST_DIR}/sangria_graphic_resource.cpp
	${CMAKE_CURRENT_LIST_DIR}/sangria_usb_keyboard.cpp
	${CMAKE_CURRENT_LIST_DIR}/sangria_battery.cpp
	${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
)

target_link_libraries( rp2040_drivers INTERFACE
	pico_stdlib
	pico_multicore
	hardware_i2c
	tinyusb_device
	tinyusb_board
)
