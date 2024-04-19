cmake_minimum_required(VERSION 3.14)
include(pico_sdk_import.cmake)

project(RP2040EEPROM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(RP2040EEPROM
	main.c
	bs_pico_at24c.c
)
target_link_libraries(RP2040EEPROM
	pico_stdlib
	hardware_i2c
)
pico_enable_stdio_usb(RP2040EEPROM 1)
pico_enable_stdio_uart(RP2040EEPROM 0)

pico_add_extra_outputs(RP2040EEPROM)
