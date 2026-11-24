#include "ems22a.h"
#include "gpio.h"
#include "pico/stdlib.h"

#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ems22a.pio.h"

#define EMS22A_SM_CLK_HZ 4000000u

// Configure PIO state machine
static pio_sm_config ems22a_sm_config(const ems22a_t *encoder) {
    pio_sm_config config = ems22a_program_get_default_config(encoder->program_offset);

    sm_config_set_sideset_pins(&config, encoder->clk_pin);
    sm_config_set_in_pins(&config, encoder->do_pin);

    // Incoming Data Configuration: Shift left, no auto-push, 32 threshold for push (unused)
    sm_config_set_in_shift(&config, false, false, 32);

    // Execution Speed: Uses RP2350 system CLK to configure 4 MHz PIO instruction rate
    sm_config_set_clkdiv(&config, (float)clock_get_hz(clk_sys) / (float)EMS22A_SM_CLK_HZ);

    return config;
}

// Initialize Encoder (Allocate PIO state machine, load program, configure GPIO pins, and apply pio_sm_config configuration)
bool ems22a_init(ems22a_t *encoder, uint32_t clk_pin, uint32_t cs_pin, uint32_t do_pin) {
    if (!encoder || encoder->initialized ||
        clk_pin >= NUM_BANK0_GPIOS ||
        cs_pin >= NUM_BANK0_GPIOS ||
        do_pin >= NUM_BANK0_GPIOS ||
        clk_pin > 31 || cs_pin > 31 || do_pin > 31 ||
        clk_pin == cs_pin ||
        clk_pin == do_pin ||
        cs_pin == do_pin) {
        return false;
    }

    // Claim PIO state machine and load program
    PIO pio;
    uint sm, offset;

    if (!pio_claim_free_sm_and_add_program(&ems22a_program, &pio, &sm, &offset)) {
        return false;
    }

    // CS Init - Idle HIGH (SIO)
    rp_gpio_write(cs_pin, true);
    rp_gpio_set_function(cs_pin, RP_GPIO_FUNC_SIO);
    rp_gpio_set_dir(cs_pin, true);

    // CLK HIGH (before PIO control since encoder default)
    rp_gpio_write(clk_pin, true);
    rp_gpio_set_function(clk_pin, RP_GPIO_FUNC_SIO);
    rp_gpio_set_dir(clk_pin, true);

    // Storing Encoder Information
    encoder->clk_pin = clk_pin;
    encoder->cs_pin = cs_pin;
    encoder->do_pin = do_pin;
    encoder->pio = pio;
    encoder->sm = sm;
    encoder->program_offset = offset;

    // Configure PIO state machine
    pio_sm_config config = ems22a_sm_config(encoder);
    pio_sm_init(pio, sm, offset, &config);

    // CLK HIGH under PIO control
    pio_sm_set_pins_with_mask(pio, sm, 1u << clk_pin, 1u << clk_pin);

    pio_sm_set_consecutive_pindirs(pio, sm, clk_pin, 1, true); // CLK output
    pio_sm_set_consecutive_pindirs(pio, sm, do_pin, 1, false); // DO input

    // PIO control over CLK and DO
    pio_gpio_init(pio, clk_pin);
    pio_gpio_init(pio, do_pin);

    pio_sm_set_enabled(pio, sm, true);

    encoder->initialized = true;
    return true;
}

// Reading Raw Data Protocol detailed in md file
bool ems22a_read_raw(ems22a_t *encoder, uint16_t *raw) {
    if (!encoder || !encoder->initialized || !raw) {
        return false;
    }

    PIO pio = encoder->pio;
    uint sm = encoder->sm;

    rp_gpio_write(encoder->cs_pin, false);
    pio_sm_put_blocking(pio, sm, 1u);

    uint32_t data = pio_sm_get_blocking(pio, sm);

    rp_gpio_write(encoder->cs_pin, true);
    busy_wait_us(1);

    *raw = (uint16_t)(data & 0xFFFFu);
    return true;
}

bool ems22a_decode(uint16_t raw, ems22a_reading_t *reading) {
    if (!reading) {
        return false;
    }

    reading->raw = raw;
    reading->position = raw >> 6; // Removing status bits and parity bit
    reading->status = (uint8_t)((raw >> 1) & 0x1Fu);

    // Check even parity (encoder uses even parity)
    uint16_t bits = raw;
    uint8_t parity = 0;

    while (bits) {
        parity ^= bits & 1u;
        bits >>= 1;
    }

    reading->parity_ok = (parity == 0);

    // OCF must be set; COF and LIN must be clear
    reading->valid = reading->parity_ok &&
                     (raw & (1u << 5)) && // OCF
                     !(raw & ((1u << 4) | (1u << 3))); // COF and LIN

    return reading->valid;
}

// Read and decode
bool ems22a_read(ems22a_t *encoder, ems22a_reading_t *reading) {
    if (!reading) {
        return false;
    }

    uint16_t raw;

    if (!ems22a_read_raw(encoder, &raw)) {
        return false;
    }

    return ems22a_decode(raw, reading);
}

// Return just the position
bool ems22a_read_position(ems22a_t *encoder, uint16_t *position) {
    if (!position) {
        return false;
    }

    ems22a_reading_t reading;

    if (!ems22a_read(encoder, &reading)) {
        return false;
    }

    *position = reading.position;
    return true;
}

// Yummy conversion to degrees (10-bit absolute encoder has 2^10=1024 positions per revolution)
float ems22a_position_to_degrees(uint16_t position) {
    return (float)position * (360.0f / 1024.0f);
}

void ems22a_deinit(ems22a_t *encoder) {
    if (!encoder || !encoder->initialized) {
        return;
    }

    // Deselect encoder and stop PIO
    rp_gpio_write(encoder->cs_pin, true);
    pio_sm_set_enabled(encoder->pio, encoder->sm, false);

    // Release PIO resources
    pio_remove_program_and_unclaim_sm(
        &ems22a_program,
        encoder->pio,
        encoder->sm,
        encoder->program_offset
    );

    // Restore CLK and DO to SIO
    rp_gpio_write(encoder->clk_pin, true);
    rp_gpio_set_function(encoder->clk_pin, RP_GPIO_FUNC_SIO);
    rp_gpio_set_dir(encoder->clk_pin, true);

    rp_gpio_set_dir(encoder->do_pin, false);
    rp_gpio_set_function(encoder->do_pin, RP_GPIO_FUNC_SIO);

    encoder->initialized = false;
}
