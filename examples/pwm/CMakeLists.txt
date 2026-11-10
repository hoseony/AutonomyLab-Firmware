add_executable(ex_pwm ex_pwm.c)
target_link_libraries(ex_pwm rp2350_drivers pico_stdlib)
pico_add_extra_outputs(ex_pwm)
