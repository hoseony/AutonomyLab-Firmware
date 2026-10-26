#include "gpio.h"
#include "pico/stdlib.h"
#include "pwm.h"
#include "resets.h"
#include "rp2350Regs.h"

#define PIN_A 2
#define PIN_B 3

static void rp_pwm_setup_pin(uint32_t pin, float duty) {
    rp_gpio_set_function(pin, RP_GPIO_FUNC_PWM);
    rp_pwm_set_duty(pin, duty);
}

int main(void) {
    /* NO ERROR CHECKING HAS BEEN DONE HERE */
    rp_reset_peripherals(RP_RESET_PWM_MASK | RP_RESET_IO_BANK0_MASK | RP_RESET_PADS_BANK0_MASK);

    // pin a and pin b shares the same frequency
    rp_pwm_set_freq(PIN_A, 20000);

    rp_pwm_setup_pin(PIN_A, 20.0f);
    rp_pwm_setup_pin(PIN_B, 70.0f);

    rp_pwm_enable(PIN_A, true);

    while (1) {
        ;
    }
}
