#include "resets.h"
#include "rp2350Regs.h"

/// Reset and wait for it to unreset
/// This is blocking.
void reset_peripherals(uint32_t mask) {
    RESETS->RESET |= mask;
    RESETS->RESET &= (~mask);

    // wait until reset done
    while ((RESETS->RESET_DONE & mask) != mask) {
        ;
    }
}
