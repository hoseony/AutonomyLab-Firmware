add_library(rp2350_drivers STATIC
    resets.c
    gpio.c
    pwm.c
    pwm_helper.c
)

target_include_directories(rp2350_drivers PUBLIC include)
