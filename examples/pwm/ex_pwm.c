#include "pico/stdlib.h"

#include "pwm.h"
#include "pwm_helper.h"
#include "gpio.h"
#include "resets.h"
#include "rp2350Regs.h"

#define PIN_A 2
#define PIN_B 3

/* some wrapper functions in progress, will move to /drivers later */

/// initialize pin to have freq_hz
bool rp_pwm_init_pin(uint32_t pin, uint32_t freq_hz) {

    return true;
}

/// for E-STOP signal, call this function
bool rp_pwm_stop(uint32_t pin) {

    return true;
}

void rp_pwm_setup_pin(uint32_t pin, float duty) {
    rp_gpio_set_function(pin, RP_GPIO_FUNC_PWM);
    rp_pwm_set_duty(pin, duty);
}

int main(void) {
    /* NO ERROR CHECKING HAS BEEN DONE HERE */
    rp_reset_peripherals(RP_RESET_PWM_MASK | RP_RESET_IO_BANK0_MASK | RP_RESET_PADS_BANK0_MASK);

    // pin a and pin b shares the same frequency
    rp_pwm_set_freq(PIN_A, 20000);

	// start at 0%
    rp_pwm_setup_pin(PIN_A, 0.0f);
    rp_pwm_setup_pin(PIN_B, 0.0f);

    rp_pwm_enable(PIN_A, true);

    while (1) {
        // 0 -> 80%
        // 4s
        rp_pwm_ramp_to(PIN_A, 80.0f, 20.0f);

        sleep_ms(1000);
        // back down to 0 wover 4s
        rp_pwm_ramp_to(PIN_A, 0.0f, 20.0f);

        sleep_ms(1000);
    }
}
