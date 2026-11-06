
#include <stdio.h>
#include <stdbool.h>

/*
    Vehicle Mode Logic Test

    Initial condition:
    Vehicle starts in STANDBY with no buttons pressed.

    Start button:
    STANDBY -> AUTONOMOUS if systems are ready.
    AUTONOMOUS -> No change.

    E Stop:
    Any mode -> STANDBY.

    This is a simulation of the mode logic only.
*/

typedef enum {
    STANDBY,
    AUTONOMOUS
} vehicle_mode;

typedef struct {
    bool start_pressed;
    bool estop_requested;
    bool systems_ready;
} mode_input;

vehicle_mode current_mode = STANDBY;

int main(void)
{
    // Initial condition
    mode_input initial_input;
    initial_input.estop_requested = false;
    initial_input.start_pressed = false;
    initial_input.systems_ready = true;

    bool previous_start = initial_input.start_pressed;

    printf("Initial Mode: STANDBY\n");

    // Simulated test cases
    mode_input tests[] = {
        {false, false, true},  // No button pressed
        {true,  false, true},  // Start pressed
        {false, false, true},  // Start released
        {true,  false, true},  // Start pressed in AUTO
        {false, false, true},  // Start released
        {false, true,  true},  // E Stop requested
        {false, false, true},  // E Stop cleared
        {true,  false, false}, // Start, system not ready
        {false, false, true},  // System ready
        {true,  false, true}   // Start pressed again
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);
    int test_index = 0;

    while (test_index < num_tests)
    {
        mode_input input = tests[test_index];

        printf("\n===== Test Case %d =====\n",
               test_index + 1);

        printf("Start Button: %d\n", input.start_pressed);
        printf("E Stop: %d\n", input.estop_requested);
        printf("Systems Ready: %d\n", input.systems_ready);

        // Detect new Start button press
        bool start_event =
            input.start_pressed && !previous_start;

        // E Stop takes priority
        if (input.estop_requested) {
            printf("E-Stop requested. Entering Standby Mode.\n");
            current_mode = STANDBY;
            input.estop_requested = false; // Reset the E-stop request after handling it
        }
        else {
            switch (current_mode) {

                case STANDBY:
                    if (start_event && input.systems_ready) {
                        current_mode = AUTONOMOUS;
                        printf("Entering Autonomous Mode from Standby Mode.\n");
                    }
                    break;

                case AUTONOMOUS:
                    // Start button has no effect in Autonomous Mode
                    if (start_event) {
                        printf("Start ignored. Already in Autonomous Mode.\n");
                    }
                    break;
            }
        }

        printf("Current Mode: %s\n",
               current_mode == AUTONOMOUS
               ? "AUTONOMOUS" : "STANDBY");

        // Save button state for next iteration
        previous_start = input.start_pressed;
        input.start_pressed = false; // Reset start button for next test case

        test_index++;
    }

    return 0;
}
