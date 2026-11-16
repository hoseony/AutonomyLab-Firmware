# Initialization
cmake_minimum_required(VERSION 3.22)

set(PICO_BOARD pico2)
set(PICO_PLATFORM rp2350)

# include pico-sdk
include(cmake/pico_sdk_import.cmake)

project(AutonomyLabFirmware LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# generate build/compile_commands.json for clangd/editor
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# initialize pico-sdk
pico_sdk_init()

# include freeRTOS
include(external/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2350_ARM_NTZ/FreeRTOS_Kernel_import.cmake)

# add subdirectory
add_subdirectory(drivers/rp2350)
add_subdirectory(drivers/devices/md30c)
add_subdirectory(rtos)
add_subdirectory(examples/pwm)
add_subdirectory(examples/md30c)
add_subdirectory(examples/rtos/blink)
