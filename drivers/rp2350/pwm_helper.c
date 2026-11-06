#include "pwm_helper.h"
#include "pico/stdlib.h"
#include "pwm.h"

#define PWM_RAMP_PERIOD_MS 10u

/// blocking: ramp pin's duty from "from" to "to" at rate "rate" [%/s]
/// returns true when done, false when something goes wrong
bool rp_pwm_ramp_to(uint32_t pin, float to, float rate) {
    // out of bound
    if (!(rate > 0.0f) || !(to >= 0.0f && to <= 100.0f)) {
        return false;
    }

    // get current duty cycle
    // (should start from wherever the PWM is right now)
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

        // update last_us
        last_us = now_us;

        // calculate max step size
        float max_step = rate * dt_s;
        float diff     = to - duty;

        // limit based on the max_step
        if (diff > max_step) {
            duty += max_step;
        } else if (diff < -max_step) {
            duty -= max_step;
        } else {
            duty = to;
        }

        // set the duty
        if (!rp_pwm_set_duty(pin, duty)) {
            return false;
        }
    }

    return true;
}
