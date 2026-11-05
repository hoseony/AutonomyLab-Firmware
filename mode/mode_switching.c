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


/*
    Current design does not give instructions on 
    how the car will move from autnomous mode to standby mode.

    when the start button is pressed, the car will enter autonomous mode if the system is ready. 
    when the start button is pressed again, the car will enter standby mode.

    when the e-stop button is pressed, the car will enter standby mode.
*/

#define START_PIN 10
#define ESTOP_PIN 11
#define AUTON_LED_PIN 14
#define ESTOP_LED_PIN 15

// Define all  modes of the vehicle
typedef enum {
    STANDBY,
    AUTONOMOUS,
    //E_STOP
} vehicle_mode;

typedef struct {
    //bool mode_selected; we don't need to select mdoe we only two modes now
    bool start_pressed;
    bool estop_requested;
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

    //read the buttons assuming active high
    input.start_pressed = gpio_get(START_PIN);
    input.estop_requested = gpio_get(ESTOP_PIN);

    //for testing we assume system always ready
    input.systems_ready = true;

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

        //Set what the current mode is
        // E-stop takes priority over every other condition
        if (input.estop_requested) {
            current_mode = STANDBY;
            input.estop_requested = false; // Reset the E-stop request after handling it
        }
        else {
            switch (current_mode) {

                case STANDBY:
                    if (start_event &&
                        input.systems_ready) {
                        current_mode = AUTONOMOUS;
                    }
                    break;

                case AUTONOMOUS:
                    if (start_event) {
                        current_mode = STANDBY;
                    }
                    break;
            }
        }
        
        // Update indicator LEDs
        gpio_put(AUTON_LED_PIN, current_mode == AUTONOMOUS); //blinking LED 
        //blinking LED will handle by Thomas

        gpio_put(ESTOP_LED_PIN, current_mode == STANDBY); //Not in autonmous mode, solid LED

        // Save button state for the next iteration
        previous_start = input.start_pressed;

        sleep_ms(20);
    }

    return 0;
}