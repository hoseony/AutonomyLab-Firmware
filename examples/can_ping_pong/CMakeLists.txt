cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico2)

# use debug build
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(REPO_ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)
set(DRIVER_DIR ${REPO_ROOT}/drivers/devices/mcp2518fd)

include(${REPO_ROOT}/cmake/pico_sdk_import.cmake)

project(can_ping_pong C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_DEOPTIMIZED_DEBUG 1)

pico_sdk_init()

# node A
add_executable(node_a
    node_a.c
    ${DRIVER_DIR}/mcp2518fd.c
)
target_include_directories(node_a PRIVATE ${DRIVER_DIR})
target_link_libraries(node_a pico_stdlib hardware_gpio hardware_spi hardware_timer)
pico_enable_stdio_uart(node_a 1)
pico_add_extra_outputs(node_a)

# node B
add_executable(node_b
    node_b.c
    ${DRIVER_DIR}/mcp2518fd.c
)
target_include_directories(node_b PRIVATE ${DRIVER_DIR})
target_link_libraries(node_b pico_stdlib hardware_gpio hardware_spi hardware_timer)
pico_enable_stdio_uart(node_b 1)
pico_add_extra_outputs(node_b)
