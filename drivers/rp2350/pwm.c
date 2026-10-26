#include "pwm.h"
#include "rp2350Regs.h"

/// turns pin# to correct slice for PWM
static uint32_t pwm_pin_to_slice(uint32_t pin) {
    if (pin < 32) {
        // slice per pin, wraps around after PWM7
        return (pin / 2) % 8;
    } else {
        return ((pin / 2) % 4) + 8;
    }
}

/// Sets PWM frequency of the pin's slice
/// Return false if out of range (9Hz to 75 MHz)
bool rp_pwm_set_freq(uint32_t pin, uint32_t freq_hz) {
    if (pin >= RP_NUM_GPIO || freq_hz == 0) {
        return false;
    }

    uint32_t ticks = RP_SYS_CLK_HZ / freq_hz;

    /* f_pwm = f_sys / ((TOP + 1)(CSR_PH_CORRECT + 1)(DIV_INT + DIV_FRAC/16))
     *       = RP_SYS_CLK_HZ / ((TOP + 1)(DIV))
     *
     * since DIV_FRAC = 0, CSR_PH_CORRECT = 0
     *
     * tick = RP_SYS_CLK_HZ / freq_hz
     *      = DIV(TOP + 1)
     * There are limit on how large DIV and TOP can be.
     *   #define RP_PWM_TOP_MAX 65534u     // 16 bits
     *   #define RP_PWM_DIV_MAX 255u       // 08 bits int
     *
     * Thus, the method that used here is first, find smallest DIV
     * that keeps TOP within its bound.
     */

    // DIV = (tick)/(TOP + 1), but it needs round up
    uint32_t div = (ticks + (RP_PWM_TOP_MAX)) / (RP_PWM_TOP_MAX + 1);

    // target too high || target too low
    if ((div < 1) || (div > RP_PWM_DIV_MAX)) {
        return false;
    }

    // TOP = (tick / div) - 1
    uint32_t steps = ticks / div;
    if (steps < 2) { // It should not be 0
        return false;
    }

    uint32_t slice = pwm_pin_to_slice(pin);

    RP_PWM->SLICE[slice].DIV = div << RP_PWM_DIV_INT_LSB;
    RP_PWM->SLICE[slice].TOP = steps - 1;

    return true;
}

/// Sets the duty cycle of the pin's PWM channel, in percent (0.0-100.0)
/// Returns false if the pin or duty is out of range.
/// Only affects the specific pin, other channel of the slice keeps its duty.
bool rp_pwm_set_duty(uint32_t pin, float duty_cycle) {
    if (pin >= RP_NUM_GPIO) {
        return false;
    }

    // reject everything out of the bound
    if (!(duty_cycle >= 0.0f && duty_cycle <= 100.0f)) {
        return false;
    }

    // CC = duty_cycle * (top + 1) / 100
    uint32_t slice = pwm_pin_to_slice(pin);
    uint32_t top   = RP_PWM->SLICE[slice].TOP;
    uint32_t cc    = duty_cycle * (top + 1) / 100;

    // [31:16] B, [15:0] A
    uint32_t channel = pin % 2;
    uint32_t shift   = channel * 16;
    uint32_t mask    = 0xFFFFu << shift;

    RP_PWM->SLICE[slice].CC = (RP_PWM->SLICE[slice].CC & ~mask) | ((cc << shift) & mask);

    return true;
}

/// enable or disable pwm for the pin
/// returns 0 if fails
bool rp_pwm_enable(uint32_t pin, bool enable) {
    if (pin >= RP_NUM_GPIO) {
        return false;
    }

    uint32_t slice = pwm_pin_to_slice(pin);

    if (enable) {
        RP_PWM->SLICE[slice].CSR |= RP_PWM_CSR_EN_MASK;
    } else {

        RP_PWM->SLICE[slice].CSR &= ~RP_PWM_CSR_EN_MASK;
    }

    return true;
}
