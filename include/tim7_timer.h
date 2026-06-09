#ifndef TIM7_TIMER_H
#define TIM7_TIMER_H

#include "registers.h"
#include "utils.h"

// =======================================================
// TIM7 - Basic timer
// =======================================================

// Control register
#define TIM7_CR1 (*(volatile uint32_t*)0x40001400)
// Counter
#define TIM7_CNT (*(volatile uint32_t*)0x40001424)
// Prescaler (to set decrement delay)
#define TIM7_PSC (*(volatile uint32_t*)0x40001428)
// Auto-reload register (to reset value after 2 bytes)
#define TIM7_ARR (*(volatile uint32_t*)0x4000142C)

// Counter enable mask
#define TIM_CR1_CEN (1 << 0)

static __always_inline void timer7_init(void)
{
    RCC_APB1ENR |= (1 << 5);

    TIM7_PSC = 8 - 1;
    TIM7_ARR = 0xFFFF;

    TIM7_CR1 |= TIM_CR1_CEN;
}

static __always_inline void delay_us(uint16_t us)
{
    TIM7_CNT = 0;

    while (TIM7_CNT < us)
    {
    }
}

#endif // TIM7_TIMER_H

