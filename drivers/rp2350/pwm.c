#include "pwm.h"

/// turns pin# to correct slice for PWM
static uint32_t PWM_pin_to_slice(uint32_t pin) {
    if (pin < 32) {
        // slice per pin, wraps around after PWM7
        return (pin / 2) % 8;
    } else {
        return ((pin / 2) % 4) + 8;
    }
}

/// This calculates DIV and TOP for PWM
static bool PWM_set_freq(uint32_t pin, uint32_t freq_hz) {
    if (pin >= NUM_GPIO || freq_hz == 0) {
        return false;
    }

    return true;
}
