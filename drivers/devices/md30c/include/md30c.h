#ifndef MD30C_H
#define MD30C_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MD30C_PWM_FREQ_HZ 20000u

typedef struct md30c_t {
    uint32_t pwm_pin;
    uint32_t dir_pin;
} md30c_t;

bool md30c_init(md30c_t *motor, uint32_t pwm_pin, uint32_t dir_pin);

#endif
