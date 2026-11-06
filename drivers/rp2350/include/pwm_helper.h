#ifndef PWM_HELPER_H
#define PWM_HELPER_H

#include "pwm.h"
#include <stdbool.h>
#include <stdint.h>

bool rp_pwm_ramp_to(uint32_t pin, float to, float rate);

#endif
