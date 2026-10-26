#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>
#include <stdint.h>

// FUNCSEL valus for RP_IO_BANK0 GPIx_CTRL[4:0]
typedef enum {
    RP_GPIO_FUNC_F0   = 0, // varies by pin
    RP_GPIO_FUNC_SPI  = 1,
    RP_GPIO_FUNC_UART = 2,
    RP_GPIO_FUNC_I2C  = 3,
    RP_GPIO_FUNC_PWM  = 4,
    RP_GPIO_FUNC_SIO  = 5,
    RP_GPIO_FUNC_PIO0 = 6,
    RP_GPIO_FUNC_PIO1 = 7,
    RP_GPIO_FUNC_PIO2 = 8,
    RP_GPIO_FUNC_F9   = 9, // varies by pin
    RP_GPIO_FUNC_USB  = 10,
    RP_GPIO_FUNC_NULL = 31,
} RP_GPIO_FUNC_t;

bool rp_gpio_set_function(uint32_t pin, RP_GPIO_FUNC_t func);

#endif
