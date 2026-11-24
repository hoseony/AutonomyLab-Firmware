// Synthetic 4x quadrature test without an encoder or motor.
// Wire GP8 -> GP6 (A) and GP9 -> GP7 (B).
// IMPORTANT: Disconnect the real encoder from GP6/GP7 during this test.
// Keep all wiring at 3.3V logic; do not connect encoder power to these wires.

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "pico/stdlib.h"
#include "gbh38.h"

#define ENCODER_A_PIN 6u
#define ENCODER_B_PIN 7u
#define STIM_A_PIN 8u
#define STIM_B_PIN 9u

// state bits: bit 0 = channel A, bit 1 = channel B.
// These orders are positive and negative for the gbh38.pio transition table
// used in this conversation. If you reverse the wiring, signs reverse.
static const uint8_t forward_states[4] = {2u, 3u, 1u, 0u};
static const uint8_t reverse_states[4] = {1u, 3u, 2u, 0u};

static void drive_state(uint8_t ab_state) {
    gpio_put(STIM_A_PIN, (ab_state & 1u) != 0u);
    gpio_put(STIM_B_PIN, (ab_state & 2u) != 0u);
    sleep_us(2000); // Keep each quadrature step stable for 2 ms.
}

static void generate_cycles(const uint8_t states[4], unsigned cycles) {
    for (unsigned revolution = 0; revolution < cycles; ++revolution) {
        for (unsigned step = 0; step < 4; ++step) {
            drive_state(states[step]);
        }
    }
}

static bool report(const char *label, int32_t expected) {
    sleep_ms(10); // Allow decoder to publish its latest counter value.
    int32_t actual = encoder_get_position();
    bool passed = actual == expected;
    printf("%s: expected=%" PRId32 ", actual=%" PRId32 " -> %s\n",
           label, expected, actual, passed ? "PASS" : "FAIL");
    fflush(stdout);
    return passed;
}

int main(void) {
    stdio_init_all();
    sleep_ms(2000);
    printf("\nGBH38 synthetic quadrature test\n");
    printf("Wire GP8 -> GP6 (A); GP9 -> GP7 (B). Disconnect real encoder.\n");

    // Initialize stimulus pins BEFORE starting decoder so both inputs begin LOW.
    gpio_init(STIM_A_PIN);
    gpio_init(STIM_B_PIN);
    gpio_put(STIM_A_PIN, false);
    gpio_put(STIM_B_PIN, false);
    gpio_set_dir(STIM_A_PIN, GPIO_OUT);
    gpio_set_dir(STIM_B_PIN, GPIO_OUT);
    sleep_ms(10);

    if (!encoder_init(ENCODER_A_PIN, ENCODER_B_PIN)) {
        printf("ERROR: encoder_init() failed.\n");
        while (true) { sleep_ms(1000); }
    }

    encoder_reset_position();
    bool all_passed = report("Initially at zero", 0);

    generate_cycles(forward_states, 10); // 4 transitions/cycle * 10 = +40
    all_passed &= report("10 forward cycles", 40);

    generate_cycles(reverse_states, 10); // -40, back to zero
    all_passed &= report("10 reverse cycles", 0);

    generate_cycles(reverse_states, 5);  // 5 * -4 = -20
    all_passed &= report("5 more reverse cycles", -20);

    generate_cycles(forward_states, 5);  // +20, back to zero
    all_passed &= report("5 more forward cycles", 0);

    printf("FINAL RESULT: %s\n", all_passed ? "ALL PASS" : "ONE OR MORE FAILURES");
    fflush(stdout);
    while (true) { sleep_ms(1000); }
}