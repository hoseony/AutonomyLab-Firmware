#include "md30c.h"
#include "gpio.h"
#include "pwm.h"
#include "pwm_helper.h"

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

bool md30c_get_speed(md30c_t *motor, float *speed) {
    if (motor == NULL) {
        return false;
    }

    return rp_pwm_get_duty(motor->pwm_pin, speed);
}

// Same deal as set_direction for this guy, abrupt speed changes until function is nonblocking
bool md30c_set_speed(md30c_t *motor, float speed) {
    if (motor == NULL) {
        return false;
    }

    return rp_pwm_set_duty(motor->pwm_pin, speed);
}

// Another evil function that shuts everything down while accelerating
bool md30c_ramp_speed(md30c_t *motor, float target, float rate) {
    if (motor == NULL) {
        return false;
    }

    return rp_pwm_ramp_to(motor->pwm_pin, target, rate);
}

// Currently does not handle stepping down and is abrupt shift, meaning motor should be already OFF when trying to change direction -> Can make it more convienent later but rp_pwm_ramp_to() needs to be made nonblocking (otherwise other important processes might get cooked)
bool md30c_set_direction(md30c_t *motor, md30c_direction_t direction) {
    float duty;

    // Hard requirement for duty cycle to be 0 (motor off) for direction to change
    if (motor == NULL ||
        (direction != MD30C_FORWARD && direction != MD30C_REVERSE) || 
        !rp_pwm_get_duty(motor->pwm_pin, &duty) || duty != 0) {
        return false;
    }

    return rp_gpio_write(motor->dir_pin, direction == MD30C_REVERSE);
}

bool md30c_stop(md30c_t *motor) {
    return md30c_set_speed(motor, 0.0f);
}

// Evil function that shuts everything else down while its slowing to a stop (cuz of the blocking wow)
bool md30c_ramp_stop(md30c_t *motor, float rate) {
    if (motor == NULL) {
        return false;
    }

    return rp_pwm_ramp_to(motor->pwm_pin, 0.0f, rate);
}