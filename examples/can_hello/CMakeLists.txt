add_executable(ex_can_hello main.c)
target_link_libraries(ex_can_hello mcp2518fd)
pico_enable_stdio_uart(ex_can_hello 1)
pico_add_extra_outputs(ex_can_hello)
