#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>
#include "utils.h"

/* ---------------- SysTick ---------------------- */
// Control and status register
#define SYST_CSR (*(volatile uint32_t*)0xE000E010)
// Reload value
#define SYST_RVR (*(volatile uint32_t*)0xE000E014)
// Current value
#define SYST_CVR (*(volatile uint32_t*)0xE000E018)

// At some time i will go insane with this naming of registers,
// i need to comment every f#cking single line to remember this shit

#define COUNTFLAG_BIT 16

// SysTick interruption, called every 1 ms
void SysTick_Handler(void);

static __always_inline void systick_init(void)
{
    SYST_RVR = 8000 - 1;
    SYST_CVR = 0;

    SYST_CSR =
        (1 << 2) | // CPU clock
        (1 << 1) | // TICKINT (SysTick_Handler caller)
        (1 << 0);  // enable
}

// Global ms counter
extern volatile uint32_t ticks;

static __always_inline void delay_ms(uint32_t ms)
{
    uint32_t start = ticks;

    while ((ticks - start) < ms)
    {
    }
}

static __always_inline uint32_t get_ticks(void)
{
    return ticks;
}

#endif // SYSTICK_H

