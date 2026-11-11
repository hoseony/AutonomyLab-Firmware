#include "encoder.h"
#include "gpio.h"
#include "resets.h"
#include "rp2350Regs.h"
#define PIO0_BASE 0x50200000u

#define PIO0_FLEVEL (*(volatile uint32_t *)(PIO0_BASE + 0x00Cu))
#define PIO0_RXF0 (*(volatile uint32_t *)(PIO0_BASE + 0x020u))
#define RP_RESET_PIO0_MASK (1u << 11)

static uint32_t encoder_zero_offset = 0;

static uint32_t encoder_get_raw_count(void) {
    uint32_t count; 
    uint32_t samples;
    // Do an extra measurement on top of whatever is already there
    samples = ((PIO0_FLEVEL >> 4) & 0xFu) + 1u;

    while (samples > 0) {
        // When you've reached that extra measurement, wait for one more (the newest measurement)
        // In the meantime, this while loop is skipped
        while (((PIO0_FLEVEL >> 4) & 0xFu) == 0) {
        }
        count = PIO0_RXF0;
        samples--;
    }
    // Return the newest measurement after draining all the old ones
    return count;
}
// Real count
int32_t encoder_get_position(void) {
    uint32_t raw_count = encoder_get_raw_count();
    return (int32_t)(raw_count - encoder_zero_offset);
}

void encoder_reset_position(void) {
    encoder_zero_offset = encoder_get_raw_count();
}

float encoder_get_revolutions(void) {
    int32_t position = encoder_get_position();
    return (float)position / ENCODER_COUNTS_PER_REV;
}

// What does encoder_init actually need to do?
bool encoder_init (uint32_t pin_a, uint32_t pin_b) {
    // Validate the GPIO pins (need to be consecutive pins for PIO program)
    if (pin_a >= RP_NUM_GPIO || pin_b >= RP_NUM_GPIO || pin_b != pin_a + 1u) {
        return false;
    }
    // Release PIO0 from reset, Hoseon already wrote this stuff
    rp_reset_peripherals(RP_RESET_PIO0_MASK);
    // Configure GPIO pins for PIO0
    if (!rp_gpio_set_function(pin_a, RP_GPIO_FUNC_PIO0) ||
        !rp_gpio_set_function(pin_b, RP_GPIO_FUNC_PIO0) ||
        !rp_gpio_set_input(pin_a, true) ||
        !rp_gpio_set_input(pin_b, true)) {
        return false;
    }
    // Load quadrature decoder instructions -> Need to write the .pio file first
    // Configure state machine 0
    // Start state machine 0
    // Establish initial position reference
    return false;
}