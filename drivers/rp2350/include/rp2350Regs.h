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
#define IO_BANK0     ((GPIO_t *)BANK_IO_BASE) // usage: IO_BANK0[pin]

typedef struct {
    volatile uint32_t STATUS;
    volatile uint32_t CTRL;
} GPIO_t;

// ------------------ PADS ---------------------
#define PADS_BANK0_BASE 0x40038000u
#define PADS_BANK0      ((PADS_BANK0_t *)PADS_BANK0_BASE)

typedef struct {
    volatile uint32_t VOLTAGE_SELECT; // voltage select. Per bank control
    volatile uint32_t GPIO[48];       // GPIO0 to GPIO47. Reset Value: 0b100010110
    volatile uint32_t SWCLK;
    volatile uint32_t SWD;
} PADS_BANK0_t;

// ------------------ PWM ---------------------

/* datasheet 12.5
 *   The rp2350 PWM block has 12 identical "slices." Each slice can drive
 *   two PWM output signals, or measure the frequency or duty cycle of an
 *   input signal. The two outputs of a slice have the same period, but
 *   independently varying duty cycles. This gives 24 total controllable PWM
 *   outputs.
 */

// datasheet: 12.5.3
// These are labeled with prefix CHxx_ in the datasheet.
typedef struct {
    volatile uint32_t CSR; // control and status register
    volatile uint32_t DIV; // counting rate := system clock frequency divided by DIV
    volatile uint32_t CTR; // direct access to the PWM counter
    volatile uint32_t CC;  // counter compare values
    volatile uint32_t TOP; // counter wrap value
} PWM_SLICE_t;

typedef struct {
    PWM_SLICE_t SLICE[12];
    volatile uint32_t EN;
    volatile uint32_t INTR;
    volatile uint32_t IRQ0_INTE;
    volatile uint32_t IRQ0_INTF;
    volatile uint32_t IRQ0_INTS;
    volatile uint32_t IRQ1_INTE;
    volatile uint32_t IRQ1_INTF;
    volatile uint32_t IRQ1_INTS;
} PWM_t;

#define PWM_BASE 0x400a8000u
#define PWM      ((PWM_t *)PWM_BASE)

/* Getting PWM working guide:
 *  release peripherals from reset
 *  set function
 *  set frequency (DIV and TOP) most likely
 *  set duty cycle (CC) I think???
 *  start the counter
 */

#endif
