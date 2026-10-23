#ifndef PWM_H
#define PWM_H

#include <stdbool.h>
#include <stdint.h>

#define NUM_GPIO    48u
#define SYS_CLK_HZ  150000000u // this is the default system clk
#define PWM_TOP_MAX 65535u     // 16 bits
#define PWM_DIV_MAX 255u       // 08 bits int

#endif
