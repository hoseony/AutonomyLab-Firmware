#include "gpio.h"
#include "pico/stdlib.h"
#include "pwm.h"
#include "resets.h"
#include "rp2350Regs.h"

#define PIN_A 2
#define PIN_B 3

static void rp_pwm_setup_pin(uint32_t pin) {
    rp_gpio_set_function(pin, RP_GPIO_FUNC_PWM);
    rp_pwm_set_duty(pin, 20.0f);
}

int main(void) {
    rp_reset_peripherals(RP_RESET_PWM_MASK | RP_RESET_IO_BANK0_MASK | RP_RESET_PADS_BANK0_MASK);

    rp_pwm_set_freq(PIN_A, 20000);

    rp_pwm_setup_pin(PIN_A);
    rp_pwm_setup_pin(PIN_B);

    rp_pwm_enable(PIN_A, true);

    while (1) {
        ;
    }
}
