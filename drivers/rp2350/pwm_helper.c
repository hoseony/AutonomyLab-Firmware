#include "pwm_helper.h"
#include "pico/stdlib.h"
#include "pwm.h"

#define PWM_RAMP_PERIOD_MS 10u

/* rtos motor task should call rp_pwm_ramp_step */
/// calculate target duty for given instance
float rp_pwm_ramp_step(float current, float target, float rate, float dt_s) {
    float max_step = rate * dt_s;
    float diff     = target - current;

    if (diff > max_step) {
        return current + max_step;
    } else if (diff < -max_step) {
        return current - max_step;
    } else {
        return target;
    }
}

/// blocking: ramp pin's duty from "from" to "to" at rate "rate" [%/s]
/// returns true when done, false when something goes wrong
bool rp_pwm_ramp_to(uint32_t pin, float to, float rate) {
    // out of bound
    if (!(rate > 0.0f) || !(to >= 0.0f && to <= 100.0f)) {
        return false;
    }

    // get current duty cycle
    // (start from wherever the PWM is right now)
    float duty;
    if (!rp_pwm_get_duty(pin, &duty)) {
        return false;
    }

    uint64_t last_us = time_us_64();

    while (duty != to) {
        sleep_ms(PWM_RAMP_PERIOD_MS);

        // calculate real elapsed time
        uint64_t now_us = time_us_64();
        float dt_s      = (now_us - last_us) / (1000000.0f);
        last_us         = now_us;

        duty = rp_pwm_ramp_step(duty, to, rate, dt_s);

        // set the duty
        if (!rp_pwm_set_duty(pin, duty)) {
            return false;
        }
    }

    return true;
}
