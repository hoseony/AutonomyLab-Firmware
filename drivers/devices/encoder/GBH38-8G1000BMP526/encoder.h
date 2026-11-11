#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include <stdbool.h>
// 1000 PPR quadrature encoder
#define ENCODER_COUNTS_PER_REV 4000

bool encoder_init(uint32_t pin_a, uint32_t pin_b);
int32_t encoder_get_position(void);
void encoder_reset_position(void);
float encoder_get_revolutions(void);

#endif