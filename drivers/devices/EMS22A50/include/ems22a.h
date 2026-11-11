#ifndef EMS22A_H
#define EMS22A_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct ems22a_t {
    uint32_t clk_pin;
    uint32_t cs_pin;
    uint32_t do_pin;
} ems22a_t;

bool ems22a_init(ems22a_t *encoder, uint32_t clk_pin, uint32_t cs_pin, uint32_t do_pin);
bool ems22a_read_raw(ems22a_t *encoder, uint16_t *raw);

#endif
