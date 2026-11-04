#include<stdio.h>
#include<stdbool.h>
#include "pico/stdlib.h"

/*
------- GPIO | Direction | Function     -----------------
------- 10   | Input     | Start Button -----------------
------- 11   | Input     | E-Stop Button -----------------
------- 14   | Output    | Autonomous Mode LED -----------------
------- 15   | Output    | E-Stop Mode LED -----------------
*/

#define START_PIN 10
#define ESTOP_PIN 11
#define AUTON_LED_PIN 14
#define ESTOP_LED_PIN 15

// Define all three modes of the vehicle
typedef enum {
    STANDBY,
    AUTONOMOUS,
    E_STOP
} vehicle_mode;

typedef struct {
    bool mode_selected;
    bool start_pressed;
    bool estop_requested;
    //bool estop_released;
    bool systems_ready;
} mode_input;

vehicle_mode current_mode = STANDBY;

void setup()
{
    // Initialize GPIO pins for input and output
    // Set START_PIN and ESTOP_PIN as input
    // Set AUTON_LED_PIN and ESTOP_LED_PIN as output
    
    //start button
    gpio_init(START_PIN);
    gpio_set_dir(START_PIN, GPIO_IN);
    gpio_pull_up(START_PIN);

    //E-stop button
    gpio_init(ESTOP_PIN);
    gpio_set_dir(ESTOP_PIN, GPIO_IN);
    gpio_pull_up(ESTOP_PIN);

    //Autonomous mode LED
    gpio_init(AUTON_LED_PIN);
    gpio_set_dir(AUTON_LED_PIN, GPIO_OUT);

    //E-stop mode LED
    gpio_init(ESTOP_LED_PIN);
    gpio_set_dir(ESTOP_LED_PIN, GPIO_OUT);


}

mode_input read_inputs()
{
    mode_input input;

    //read the buttons
    input.start_pressed = !gpio_get(START_PIN);
    input.estop_requested = !gpio_get(ESTOP_PIN);

    //for testing
    input.systems_ready = true;
    input.mode_selected = true;

    return input;
}

int main(void)
{
    stdio_init_all();
    setup();

    mode_input initial_input = read_inputs();
    bool previous_start = initial_input.start_pressed;

    while (true)
    {
        mode_input input = read_inputs();

        // Detect a new Start button press
        bool start_event =
            input.start_pressed && !previous_start;

        // E-stop takes priority over every other condition
        if (input.estop_requested) {
            current_mode = E_STOP;
        }
        else {
            switch (current_mode) {

                case STANDBY:
                    if (input.mode_selected &&
                        start_event &&
                        input.systems_ready) {

                        current_mode = AUTONOMOUS;
                    }
                    break;

                case AUTONOMOUS:
                    if (!input.mode_selected ||
                        !input.systems_ready) {

                        current_mode = STANDBY;
                    }
                    break;

                case E_STOP:
                    // Require a new Start press to rearm
                    // Return to STANDBY, not AUTONOMOUS
                    if (start_event) {
                        current_mode = STANDBY;
                    }
                    break;

                default:
                    current_mode = E_STOP;
                    break;
            }
        }
        
        // Update indicator LEDs
        gpio_put(AUTON_LED_PIN, current_mode == AUTONOMOUS);
        gpio_put(ESTOP_LED_PIN, current_mode == E_STOP);

        // Save button state for the next iteration
        previous_start = input.start_pressed;

        sleep_ms(20);
    }

    return 0;
}