#include "ems22a.h"
#include "gpio.h"
#include "pico/stdlib.h"

#define EMS22A_CLOCK_DELAY_US 2u

bool ems22a_init(ems22a_t *encoder, uint32_t clk_pin, uint32_t cs_pin, uint32_t do_pin) {
    if (!encoder || clk_pin > 31 || cs_pin > 31 || do_pin > 31 || clk_pin == cs_pin || clk_pin == do_pin || cs_pin == do_pin) {
        return false;
    }

    if (!rp_gpio_write(cs_pin, true) || !rp_gpio_set_function(cs_pin, RP_GPIO_FUNC_SIO) || !rp_gpio_set_dir(cs_pin, true)) {
        return false;
    }

    if (!rp_gpio_write(clk_pin, true) || !rp_gpio_set_function(clk_pin, RP_GPIO_FUNC_SIO) || !rp_gpio_set_dir(clk_pin, true)) {
        return false;
    }

    if (!rp_gpio_set_function(do_pin, RP_GPIO_FUNC_SIO) || !rp_gpio_set_dir(do_pin, false)) {
        return false;
    }

    encoder->clk_pin = clk_pin;
    encoder->cs_pin = cs_pin;
    encoder->do_pin = do_pin;

    return true;
}

bool ems22a_read_raw(ems22a_t *encoder, uint16_t *raw) {
    if (encoder == NULL || raw == NULL) {
        return false;
    }

    uint16_t data = 0;

    // Probably have to keep implementing here


}