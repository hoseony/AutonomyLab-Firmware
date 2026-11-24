// Manual GBH38 test: move the encoder shaft by hand and watch the count.
// Encoder channel A -> GP6, channel B -> GP7 (with safe logic levels).
// Send 'z' on USB serial to make the current position zero.

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "gbh38.h"

#define ENCODER_A_PIN 6u
#define ENCODER_B_PIN 7u

int main(void) {
    stdio_init_all();
    sleep_ms(2000); // Give the USB serial monitor time to attach.

    printf("\nGBH38 manual encoder test\n");
    printf("A=GP%u, B=GP%u; turn the shaft by hand; send z to zero.\n", ENCODER_A_PIN, ENCODER_B_PIN);

    if (!encoder_init(ENCODER_A_PIN, ENCODER_B_PIN)) {
        printf("ERROR: encoder_init() failed.\n");
        while (true) { sleep_ms(1000); }
    }

    printf("Initialized. Expected: 4000 counts per encoder shaft revolution (4x).\n");
    while (true) {
        int ch = getchar_timeout_us(0);
        if (ch == 'z' || ch == 'Z') {
            encoder_reset_position();
            printf("Zero position reset.\n");
        }

        int32_t count = encoder_get_position();
        float rev = (float)count / (float)ENCODER_COUNTS_PER_REV;
        float degrees = rev * 360.0f;
        printf("count=%" PRId32 "  revolutions=%.4f  degrees=%.2f\n", count, (double)rev, (double)degrees);
        fflush(stdout);
        sleep_ms(250);
    }
}

