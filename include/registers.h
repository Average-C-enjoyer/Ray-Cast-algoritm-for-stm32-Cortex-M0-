#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>
#include "utils.h"

// ===========================================================
// RCC macros
// ===========================================================

// RCC - Reset and Clock control
// Advanced High-performance Bus Enable register
#define RCC_AHBENR  (*(volatile uint32_t*)0x40021014)

// APB peripheral clock enable register 1
// (address of register to enable USART2)
#define RCC_APB1ENR (*(volatile uint32_t*)0x4002101C)

// USART2 enable is 17th bit (USART2 EN)
#define rcc_enable_usart2() RCC_APB1ENR |= (1 << 17)
// Enable GPIO port A is 17th bit (GPIOA EN)
#define rcc_enable_gpioa()  RCC_AHBENR  |= (1 << 17)


// ===========================================================
// GPIO MODER
// ===========================================================

// GPIOA Mode register address
#define GPIOA_MODER (*(volatile uint32_t*)0x48000000)
// GPIOA Output Data register address
#define GPIOA_ODR   (*(volatile uint32_t*)0x48000014)


#define gpio_moder_set(moder_addr, pin, mode) do{ \
    /* Clear the pin moder */                     \
    moder_addr &= ~(3 << (pin * 2));              \
    /* Set the chosen mode */                     \
    moder_addr |= (mode << (pin * 2));            \
} while(0)

#define GPIO_MODE_INPUT    0
#define GPIO_MODE_OUTPUT   1
#define GPIO_MODE_ALT_FUNC 2
#define GPIO_MODE_ANALOG   3


// ===========================================================
// GPIO alternate function control
// ===========================================================

// GPIO alternate function low address (for pins PA0 to PA7)
#define GPIOA_AFRL (*(volatile uint32_t*)0x48000020)

// GPIOA alternate function high address (for pins PA8 to PA15)
#define GPIOA_AFRH (*(volatile uint32_t*)0x48000024)

#define AF1 1

#define gpio_afr_set(afr_addr, pin, field) do { \
    afr_addr &= ~(0xF << (pin * 4));            \
    afr_addr |= (field << (pin * 4));           \
} while(0)


// ===========================================================
// NVIC
// ===========================================================

// NVIC line
#define NVIC_ISER (*(volatile uint32_t*)0xE000E100)

// USART2 interrupt has 28th number in vector table (IRQ28)
#define nvic_enable_usart2() NVIC_ISER |= (1u << 28)

/* ---------------- CPU Pins --------------------- */
#define PA1 1
#define PA2 2
#define PA3 3
#define PA5 5

#endif // REGISTERS_H

