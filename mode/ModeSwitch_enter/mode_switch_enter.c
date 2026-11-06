
#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"

/*
    GPIO Configuration

    GPIO 11: E Stop status input
    GPIO 14: Autonomous Mode LED
    GPIO 15: Standby Mode LED

    Start Command: Enter key through USB serial

    Initial Mode: STANDBY

    Enter:
        STANDBY -> AUTONOMOUS if systems ready
        AUTONOMOUS -> No change

    E Stop status:
        Any mode -> STANDBY

    This is a firmware development test.
    The actual E Stop must operate independently
    through a hardware safety circuit.
*/

#define ESTOP_PIN 11
#define AUTON_LED_PIN 14
#define STANDBY_LED_PIN 15

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

void setup(void)
{
    // E Stop status input
    gpio_init(ESTOP_PIN);
    gpio_set_dir(ESTOP_PIN, GPIO_IN);
    gpio_pull_up(ESTOP_PIN);

    // Autonomous LED
    gpio_init(AUTON_LED_PIN);
    gpio_set_dir(AUTON_LED_PIN, GPIO_OUT);

    // Standby LED or you can call Estop LED
    gpio_init(STANDBY_LED_PIN);
    gpio_set_dir(STANDBY_LED_PIN, GPIO_OUT);

    // Initial LED states
    gpio_put(AUTON_LED_PIN, false);
    gpio_put(STANDBY_LED_PIN, true);
}

// Read USB serial input without blocking
//this is something I got help with ai about how to use the enter key to start the car
bool read_start_command(void)
{
    static bool previous_was_cr = false;
    bool start_requested = false;

    int ch;

    while ((ch = getchar_timeout_us(0)) != PICO_ERROR_TIMEOUT)
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

    // Enter key from USB serial
    input.start_requested = read_start_command();

    // Assumes LOW means E Stop requested
    input.estop_requested = !gpio_get(ESTOP_PIN);

    // Temporary testing condition
    input.systems_ready = true;

    return input;
}

int main(void)
{
    stdio_init_all();
    setup();

    // Allow USB serial connection to establish
    sleep_ms(2000);

    printf("\n===== Vehicle Mode Control =====\n");
    printf("Initial Mode: STANDBY\n");
    printf("Press Enter to request Autonomous Mode.\n");

    while (true)
    {
        mode_input input = read_inputs();

        // E Stop takes priority
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
                        printf("Entering Autonomous Mode.\n");
                    }
                    break;

                case AUTONOMOUS:
                    // Ignore additional Start commands
                    if (input.start_requested) {
                        printf("Already in Autonomous Mode.\n");
                    }
                    break;
            }
        }

        // Update LED indicators
        gpio_put(AUTON_LED_PIN,
                 current_mode == AUTONOMOUS);

        gpio_put(STANDBY_LED_PIN,
                 current_mode == STANDBY);

        sleep_ms(20);
    }

    return 0;
}
