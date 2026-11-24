#include <stdio.h>

#include "ems22a.h"
#include "pico/stdlib.h"

// Encoder pins
#define PIN_CLK 2
#define PIN_CS  3
#define PIN_DO  4

void error_loop() {
    while (1) {
        ;
    }
}

int main(void) {
    stdio_init_all();
    sleep_ms(2000);

    ems22a_t encoder = {0};

    if (!ems22a_init(&encoder, PIN_CLK, PIN_CS, PIN_DO)) {
        printf("Encoder initialization failed!\n");
        error_loop();
    }

    printf("Encoder initialized!\n");

    while (1) {
        uint16_t raw;
        ems22a_reading_t reading;

        if (!ems22a_read_raw(&encoder, &raw)) {
            printf("Encoder read failed!\n");
            error_loop();
        }

        bool valid = ems22a_decode(raw, &reading);

        printf("Raw: 0x%04X | Position: %u | Angle: %.2f | Parity: %s | Valid: %s\n",
               (unsigned)reading.raw,
               (unsigned)reading.position,
               (double)ems22a_position_to_degrees(reading.position),
               reading.parity_ok ? "OK" : "FAIL",
               valid ? "YES" : "NO");

        sleep_ms(250);
    }
}
