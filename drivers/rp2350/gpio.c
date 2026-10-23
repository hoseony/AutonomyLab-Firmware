#include "gpio.h"
#include "rp2350Regs.h"

bool gpio_set_fnction(uint32_t pin, GPIO_FUNC_t func) {
    /* select function */
    if (pin >= 48u) {
        return false;
    }

    // this is fine because 0 is "NORMAL"
    IO_BANK0[pin].CTRL = func;

    /* clear OD and ISO in the pads */
    PADS_BANK0->GPIO[pin] &= ~((0x1u << 7) | (0x1u << 8));

    return true;
}
