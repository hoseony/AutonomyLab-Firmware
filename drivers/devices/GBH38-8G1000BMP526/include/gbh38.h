
#ifndef GBH38_H
#define GBH38_H

#include <stdint.h>
#include <stdbool.h>

// 1000 PPR encoder, 4x quadrature decoding
#define ENCODER_COUNTS_PER_REV 4000u

bool encoder_init(uint32_t pin_a, uint32_t pin_b);

int32_t encoder_get_position(void);

void encoder_reset_position(void);

float encoder_get_revolutions(void);

#endif
