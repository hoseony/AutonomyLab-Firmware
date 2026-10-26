#ifndef PWM_H
#define PWM_H

#include <stdbool.h>
#include <stdint.h>

#define RP_PWM_TOP_MAX 65534u     // 16 bits (65535 - 1 to make duty fit..)
#define RP_PWM_DIV_MAX 255u       // 08 bits int

#define RP_PWM_DIV_INT_LSB 4 // [11:4] int, [3:0] frac

bool rp_pwm_set_freq(uint32_t pin, uint32_t freq_hz);
bool rp_pwm_set_duty(uint32_t pin, float duty_cycle);
bool rp_pwm_enable(uint32_t pin, bool enable);

#endif
