
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "pico/stdlib.h"

/*
    Vehicle Mode Control

    Input:
        GPIO 11: E Stop status input
        Enter key: Start command through USB serial

    Output:
        Version 1: GPIO 25, onboard LED
        Version 2: GPIO 14, external LED

    Modes:
        STANDBY: LED solid ON
        AUTONOMOUS: LED blinking

    Initial Mode: STANDBY

    Enter:
        STANDBY -> AUTONOMOUS if systems ready
        AUTONOMOUS -> No change

    E Stop status:
        Any mode -> STANDBY

    The physical E Stop must operate independently
    through a hardware safety circuit.
*/

#define ESTOP_PIN 11

#ifndef MODE_LED_PIN
#define MODE_LED_PIN 25
#endif

#define BLINK_INTERVAL_MS 50

typedef enum {
    STANDBY,
    AUTONOMOUS
} vehicle_mode;

typedef struct {
    bool start_requested;
    bool estop_requested;
    bool systems_ready;
} mode_input;

vehicle_mode current_mode = STANDBY;

bool led_state = true;
uint64_t last_toggle_time = 0;

void setup(void)
{
    // E Stop status input
    gpio_init(ESTOP_PIN);
    gpio_set_dir(ESTOP_PIN, GPIO_IN);
    gpio_pull_up(ESTOP_PIN);

    // Mode indication LED
    gpio_init(MODE_LED_PIN);
    gpio_set_dir(MODE_LED_PIN, GPIO_OUT);

    // Initial mode is STANDBY
    gpio_put(MODE_LED_PIN, true);
    led_state = true;
    last_toggle_time = time_us_64();
}

// Read USB serial input without blocking
bool read_start_command(void)
{
    static bool previous_was_cr = false;
    bool start_requested = false;

    int ch;

    while ((ch = getchar_timeout_us(0))
           != PICO_ERROR_TIMEOUT)
    {
        if (ch == '\r') {
            start_requested = true;
            previous_was_cr = true;
        }
        else if (ch == '\n') {
            if (!previous_was_cr) {
                start_requested = true;
            }
            previous_was_cr = false;
        }
        else {
            previous_was_cr = false;
        }
    }

    return start_requested;
}

mode_input read_inputs(void)
{
    mode_input input;

    input.start_requested = read_start_command();

    // Active LOW E Stop status
    input.estop_requested = !gpio_get(ESTOP_PIN);

    // Temporary testing condition
    input.systems_ready = true;

    return input;
}

void update_mode_led(void)
{
    if (current_mode == STANDBY) {
        // Solid LED in standby
        led_state = true;
        gpio_put(MODE_LED_PIN, true);
    }
    else if (current_mode == AUTONOMOUS) {

        uint64_t current_time = time_us_64();

        if (current_time - last_toggle_time >=
            BLINK_INTERVAL_MS * 1000ULL)
        {
            led_state = !led_state;
            gpio_put(MODE_LED_PIN, led_state);

            last_toggle_time = current_time;
        }
    }
}

int main(void)
{
    stdio_init_all();
    setup();

    sleep_ms(20);

    printf("\n===== Vehicle Mode Control =====\n");
    printf("Initial Mode: STANDBY\n");
    printf("Press Enter to request Autonomous Mode.\n");

    while (true)
    {
        mode_input input = read_inputs();

        // E Stop status takes priority
        if (input.estop_requested) {
            if (current_mode != STANDBY) {
                current_mode = STANDBY;
                printf("E Stop requested. Entering Standby.\n");
            }
        }
        else {
            switch (current_mode) {

                case STANDBY:
                    if (input.start_requested &&
                        input.systems_ready) {

                        current_mode = AUTONOMOUS;

                        // Begin blinking immediately
                        led_state = true;
                        gpio_put(MODE_LED_PIN, true);
                        last_toggle_time = time_us_64();

                        printf("Entering Autonomous Mode.\n");
                    }
                    break;

                case AUTONOMOUS:
                    if (input.start_requested) {
                        printf("Already in Autonomous Mode.\n");
                    }
                    break;
            }
        }

        // Update LED based on current mode
        update_mode_led();
        //printf("Current Mode: %s\n",current_mode == STANDBY ? "STANDBY" : "AUTONOMOUS");

        sleep_ms(20);
    }

    return 0;
}
