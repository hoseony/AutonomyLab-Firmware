cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(mode_switch C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(mode_switch
    mode_switch.c
)

target_link_libraries(mode_switch
    pico_stdlib
)

pico_enable_stdio_usb(mode_switch 0)
pico_enable_stdio_uart(mode_switch 1)

pico_add_extra_outputs(mode_switch)