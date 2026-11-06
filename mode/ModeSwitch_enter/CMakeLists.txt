cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico2)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(mode_switch_enter C CXX ASM)

pico_sdk_init()

add_executable(mode_switch_enter
    mode_switch_enter.c
)

target_link_libraries(mode_switch_enter
    pico_stdlib
)

# Enable USB serial for Enter key input
pico_enable_stdio_usb(mode_switch_enter 1)
pico_enable_stdio_uart(mode_switch_enter 0)

# Generate ELF, UF2 and other firmware outputs
pico_add_extra_outputs(mode_switch_enter)
