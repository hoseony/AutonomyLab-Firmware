#include "md30c.h"
#include "gpio.h"
#include "pwm.h"

bool md30c_init(md30c_t *motor, uint32_t pwm_pin, uint32_t dir_pin) {
    if (motor == NULL || pwm_pin == dir_pin || pwm_pin > 31 || dir_pin > 31) {
        return false;
    }

    // Turn everything off because bad
    if (!rp_gpio_write(dir_pin, false) ||
        !rp_gpio_set_function(dir_pin, RP_GPIO_FUNC_SIO) ||
        !rp_gpio_set_dir(dir_pin, true)
        ) {

        // Trying to clean but this is kinda weird and bad probably
        rp_gpio_write(dir_pin, false);
        rp_gpio_set_dir(dir_pin, true);
        rp_gpio_set_function(dir_pin, RP_GPIO_FUNC_SIO);

        return false;
    }

    // Init PWM to 20kHz and 0 doody
    // PWM Channel sharing slice share frequency might have to deal with that at some point but not today baby
    if (!rp_pwm_set_freq(pwm_pin, MD30C_PWM_FREQ_HZ) ||
        !rp_pwm_set_duty(pwm_pin, 0.0f) ||
        !rp_pwm_enable(pwm_pin, true) ||
        !rp_gpio_set_function(pwm_pin, RP_GPIO_FUNC_PWM)
        ) {

        // Trying to clean but this is kinda weird and bad probably but this one is a little more intelligent I think
        rp_gpio_write(pwm_pin, false);
        rp_gpio_set_dir(pwm_pin, true);
        rp_gpio_set_function(pwm_pin, RP_GPIO_FUNC_SIO);

        return false;
    }

    // Only store pin after everything awesome
    motor->pwm_pin = pwm_pin;
    motor->dir_pin = dir_pin;

    return true;
}