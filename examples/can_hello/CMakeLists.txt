cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico2)

# use debug build
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(REPO_ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)
set(DRIVER_DIR ${REPO_ROOT}/drivers/devices/mcp2518fd)

include(${REPO_ROOT}/cmake/pico_sdk_import.cmake)

project(can_hello C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_DEOPTIMIZED_DEBUG 1)

pico_sdk_init()

add_executable(can_hello
    main.c
    ${DRIVER_DIR}/mcp2518fd.c
)

target_include_directories(can_hello PRIVATE ${DRIVER_DIR})

target_link_libraries(can_hello
    pico_stdlib
    hardware_gpio
    hardware_spi
    hardware_timer
)

target_compile_definitions(can_hello PRIVATE PICO_DEBUG=1)

pico_enable_stdio_uart(can_hello 1)
pico_add_extra_outputs(can_hello)