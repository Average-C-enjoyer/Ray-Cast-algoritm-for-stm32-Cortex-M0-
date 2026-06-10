
#ifndef TIM7_TIMER_H
#define TIM7_TIMER_H

#include "registers.h"
#include "utils.h"

// =======================================================
// TIM17 - Basic timer
// =======================================================
#define TIM17_BASE 0x40014800

// Control register
#define TIM17_CR1 (*(volatile uint32_t*)(TIM17_BASE + 0x00))
// Counter
#define TIM17_CNT (*(volatile uint32_t*)(TIM17_BASE + 0x24))
// Prescaler (to set decrement delay)
#define TIM17_PSC (*(volatile uint32_t*)(TIM17_BASE + 0x28))
// Auto-reload register (to reset value after 2 bytes)
#define TIM17_ARR (*(volatile uint32_t*)(TIM17_BASE + 0x2C))

// Counter enable mask
#define TIM_CR1_CEN (1 << 0)

static __always_inline void timer17_init(void)
{
    RCC_APB2ENR |= (1 << 18);

    TIM17_PSC = 8 - 1;
    TIM17_ARR = 0xFFFF;

    TIM17_CR1 |= TIM_CR1_CEN;
}

static __always_inline void delay_us(uint16_t us)
{
    TIM17_CNT = 0;

    while (TIM17_CNT < us)
    {
    }
}

#endif // TIM7_TIMER_H


