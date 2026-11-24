add_executable(ex_ems22a ex_ems22a.c)

target_link_libraries(ex_ems22a ems22a pico_stdlib)

pico_enable_stdio_usb(ex_ems22a 1)
pico_enable_stdio_uart(ex_ems22a 0)

pico_add_extra_outputs(ex_ems22a)
