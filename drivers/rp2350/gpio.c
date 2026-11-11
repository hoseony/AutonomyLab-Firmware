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

/* Drive pin high or low */
bool rp_gpio_write(uint32_t pin, bool high) {
    // Only for GPIO 0-31 for now IDK if we need the others but can just add the other register if needed
    if (pin > 31) {
        return false;
    }

    if (high) {
        *(volatile uint32_t*) RP_GPIO_OUT_SET = 1u << pin;
    }
    else {
        *(volatile uint32_t*) RP_GPIO_OUT_CLR = 1u << pin;
    }

    return true;
}

/* enable or disable pin's output */
bool rp_gpio_set_dir(uint32_t pin, bool output) {
    // Same thing as rp_gpio_write for these registers
    if (pin > 31) {
        return false;
    }

    if (output) {
        *(volatile uint32_t*) RP_GPIO_OE_SET = 1u << pin;
    }
    else {
        *(volatile uint32_t*) RP_GPIO_OE_CLR = 1u << pin;
    }

    return true;
}

/* enable or disable input */
bool rp_gpio_set_input(uint32_t pin, bool enable) {
    if (pin > 31) {
        return false;
    }

    if (enable) {
        RP_PADS_BANK0->GPIO[pin] |= RP_PADS_IE_MASK;
    } else {
        RP_PADS_BANK0->GPIO[pin] &= ~ RP_PADS_IE_MASK;
    }

    return true;
}

bool rp_gpio_read(uint32_t pin, bool *high) {
    if (pin > 31 || !high) {
        return false;
    }

    *high = (*(volatile uint32_t*) RP_GPIO_IN >> pin) & 0x1u;
    return true;
}
