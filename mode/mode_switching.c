#include<stdio.h>
#include<stdbool.h>

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


int main()
{
    
    return 0;
}