
#ifndef EMS22A_H
#define EMS22A_H

#include <stdint.h>
#include <stdbool.h>

#include "hardware/pio.h"

typedef struct ems22a_t {
    uint32_t clk_pin;
    uint32_t cs_pin;
    uint32_t do_pin;

    PIO pio;
    uint sm;
    uint program_offset;

    bool initialized;
} ems22a_t;

typedef struct ems22a_reading_t {
    uint16_t raw;
    uint16_t position;
    uint8_t status;
    bool parity_ok;
    bool valid;
} ems22a_reading_t;

// Initialization and cleanup
bool ems22a_init(ems22a_t *encoder, uint32_t clk_pin, uint32_t cs_pin, uint32_t do_pin);
void ems22a_deinit(ems22a_t *encoder);

// Reading and decoding
bool ems22a_read_raw(ems22a_t *encoder, uint16_t *raw);
bool ems22a_decode(uint16_t raw, ems22a_reading_t *reading);
bool ems22a_read(ems22a_t *encoder, ems22a_reading_t *reading);
bool ems22a_read_position(ems22a_t *encoder, uint16_t *position);

// Conversion
float ems22a_position_to_degrees(uint16_t position);

#endif
