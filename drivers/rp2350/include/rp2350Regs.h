#ifndef RP2350REGS_H
#define RP2350REGS_H

// ------------------ Include ---------------------
#include <stdint.h>

// ------------------ RESET ---------------------
#define RESET_BASE 0x40020000u
#define RESETS     ((RESETS_t *)RESET_BASE)

#define RESET_PWM_MASK        (0x00000001u << 16)
#define RESET_IO_BANK0_MASK   (0x00000001u << 6)
#define RESET_PADS_BANK0_MASK (0x00000001u << 9)

typedef struct {
    volatile uint32_t RESET;
    volatile uint32_t WDSEL;
    volatile uint32_t RESET_DONE;
} RESETS_t;

// ------------------ GPIO ---------------------
#define BANK_IO_BASE 0x40028000u
#define IO_BANK0     ((GPIO_t *)BANK_IO_BASE) /* usage: IO_BANK0[pin] */

typedef struct {
    volatile uint32_t STATUS;
    volatile uint32_t CTRL;
} GPIO_t;

// ------------------ PADS ---------------------
#define PADS_BANK0_BASE 0x40038000u
#define PADS_BANK0      ((PADS_BANK0_t *)PADS_BANK0_BASE)

typedef struct {
    volatile uint32_t VOLTAGE_SELECT; /* voltage select. Per bank control */
    volatile uint32_t GPIO[48];       /* GPIO0 to GPIO47. Reset Value: 0b100010110 */
    volatile uint32_t SWCLK;
    volatile uint32_t SWD;
} PADS_BANK0_t;

#endif
