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

typedef enum {
    MD30C_FORWARD, // 0
    MD30C_REVERSE // 1
} md30c_direction_t;

bool md30c_init(md30c_t *motor, uint32_t pwm_pin, uint32_t dir_pin);
bool md30c_get_speed(md30c_t *motor, float *speed);
bool md30c_set_speed(md30c_t *motor, float speed);
bool md30c_ramp_speed(md30c_t *motor, float target, float rate);
bool md30c_set_direction(md30c_t *motor, md30c_direction_t direction);
bool md30c_stop(md30c_t *motor);
bool md30c_ramp_stop(md30c_t *motor, float rate);

#endif
