#include "gpio.h"
#include "rp2350Regs.h"

bool rp_gpio_set_function(uint32_t pin, RP_GPIO_FUNC_t func) {
    /* select function */
    if (pin >= RP_NUM_GPIO) {
        return false;
    }

    // this is fine because 0 is "NORMAL"
    RP_IO_BANK0[pin].CTRL = func;

    /* clear OD and ISO in the pads */
    RP_PADS_BANK0->GPIO[pin] &= ~(RP_PADS_GPIO_OD_MASK | RP_PADS_GPIO_ISO_MASK);

    return true;
}
