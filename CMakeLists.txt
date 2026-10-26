# Initialization
cmake_minimum_required(VERSION 3.14)

set(PICO_BOARD pico2)
set(PICO_PLATFORM rp2350)

include(cmake/pico_sdk_import.cmake)

project(AutonomyLabFirmware VERSION 0.1 LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pico_sdk_init()

# add subdirectory
add_subdirectory(drivers/rp2350)
add_subdirectory(examples/pwm)
