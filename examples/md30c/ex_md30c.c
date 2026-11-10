#include "md30c.h"
#include "pwm.h"
#include "resets.h"
#include "rp2350Regs.h"

// pwm pins
#define PIN_A 2
#define PIN_B 3

// direction pin
#define PIN_C 4

void error_loop() {
    while (1) {
        ;
    }
}

int main(void) {
    rp_reset_peripherals(RP_RESET_PWM_MASK | RP_RESET_IO_BANK0_MASK | RP_RESET_PADS_BANK0_MASK);

    md30c_t motor;

    if (!md30c_init(&motor, PIN_A, PIN_C)) {
        error_loop();
    }

    if (!md30c_set_direction(&motor, MD30C_FORWARD)) {
        error_loop();
    }

    float speed;
    if (!md30c_get_speed(&motor, &speed)) {
        error_loop();
    }

    while (1) {
        if (!md30c_ramp_speed(&motor, 100.0, 5.0f)) {
            error_loop();
        }

        if (!md30c_ramp_speed(&motor, 0.0, 5.0f)) {
            error_loop();
        }

        if (!md30c_set_direction(&motor, MD30C_REVERSE)) {
            error_loop();
        }

        if (!md30c_ramp_speed(&motor, 100.0, 5.0f)) {
            error_loop();
        }

        if (!md30c_ramp_speed(&motor, 0.0, 5.0f)) {
            error_loop();
        }

        if (!md30c_set_direction(&motor, MD30C_FORWARD)) {
            error_loop();
        }
    }
}
