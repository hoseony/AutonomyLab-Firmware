#ifndef RP2350REGS_H
#define RP2350REGS_H

// ------------------ Include ---------------------
#include <stdint.h>

// ------------------ Chip ---------------------
#define RP_NUM_GPIO   48u
#define RP_SYS_CLK_HZ 150000000u // this is the default system clk

// ------------------ RESET ---------------------
#define RP_RESET_BASE 0x40020000u
#define RP_RESETS     ((RP_RESETS_t *)RP_RESET_BASE)

#define RP_RESET_PWM_MASK        (0x00000001u << 16)
#define RP_RESET_IO_BANK0_MASK   (0x00000001u << 6)
#define RP_RESET_PADS_BANK0_MASK (0x00000001u << 9)

typedef struct {
    volatile uint32_t RESET;
    volatile uint32_t WDSEL;
    volatile uint32_t RESET_DONE;
} RP_RESETS_t;

// ------------------ GPIO ---------------------
#define RP_BANK_IO_BASE 0x40028000u
#define RP_IO_BANK0     ((RP_GPIO_t *)RP_BANK_IO_BASE) // usage: RP_IO_BANK0[pin]

typedef struct {
    volatile uint32_t STATUS;
    volatile uint32_t CTRL;
} RP_GPIO_t;

// ------------------ PADS ---------------------
#define RP_PADS_BANK0_BASE 0x40038000u
#define RP_PADS_BANK0      ((RP_PADS_BANK0_t *)RP_PADS_BANK0_BASE)

// PADS_BANK0 GPIOx register bits
#define RP_PADS_GPIO_OD_MASK  (0x1u << 7) // output disable
#define RP_PADS_GPIO_ISO_MASK (0x1u << 8) // pad isolation (set at reset)

typedef struct {
    volatile uint32_t VOLTAGE_SELECT; // voltage select. Per bank control
    volatile uint32_t GPIO[48];       // GPIO0 to GPIO47. Reset Value: 0b100010110
    volatile uint32_t SWCLK;
    volatile uint32_t SWD;
} RP_PADS_BANK0_t;

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
} RP_PWM_SLICE_t;

typedef struct {
    RP_PWM_SLICE_t SLICE[12];
    volatile uint32_t EN;
    volatile uint32_t INTR;
    volatile uint32_t IRQ0_INTE;
    volatile uint32_t IRQ0_INTF;
    volatile uint32_t IRQ0_INTS;
    volatile uint32_t IRQ1_INTE;
    volatile uint32_t IRQ1_INTF;
    volatile uint32_t IRQ1_INTS;
} RP_PWM_t;

#define RP_PWM_BASE        0x400a8000u
#define RP_PWM             ((RP_PWM_t *)RP_PWM_BASE)
#define RP_PWM_CSR_EN_MASK (0x1u << 0)

/* Getting PWM working guide:
 *  release peripherals from reset
 *  set function
 *  set frequency (DIV and TOP) most likely
 *  set duty cycle (CC) I think???
 *  start the counter
 */

#endif
