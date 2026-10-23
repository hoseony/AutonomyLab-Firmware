#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>
#include <stdint.h>

// FUNCSEL valus for IO_BANK0 GPIx_CTRL[4:0]
typedef enum {
    GPIO_FUNC_F0   = 0, // varies by pin
    GPIO_FUNC_SPI  = 1,
    GPIO_FUNC_UART = 2,
    GPIO_FUNC_I2C  = 3,
    GPIO_FUNC_PWM  = 4,
    GPIO_FUNC_SIO  = 5,
    GPIO_FUNC_PIO0 = 6,
    GPIO_FUNC_PIO1 = 7,
    GPIO_FUNC_PIO2 = 8,
    GPIO_FUNC_F9   = 9, // varies by pin
    GPIO_FUNC_USB  = 10,
    GPIO_FUNC_NULL = 31,
} GPIO_FUNC_t;

#endif
