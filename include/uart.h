#ifndef UART_H
#define UART_H

#include "registers.h"
#include "utils.h"

// Control register 1
#define USART2_CR1 (*(volatile uint32_t*)0x40004400)
// Baud Rate Register
#define USART2_BRR (*(volatile uint32_t*)0x4000440C)
// Interrupt and Status Register
#define USART2_ISR (*(volatile uint32_t*)0x4000441C)
// Recieve data register
#define USART2_RDR (*(volatile uint32_t*)0x40004424)
// Transmit data register
#define USART2_TDR (*(volatile uint32_t*)0x40004428)

/* CR1 bits */

// CR1 USART enable
#define USART_CR1_UE (1 << 0)
// CR1 Receiver Enable (RX)
#define USART_CR1_RE (1 << 2)
// CR1 transmitter enable (TX)
#define USART_CR1_TE (1 << 3)

/* ISR bits */

// Transmit data register empty (data ready to write)
#define USART_ISR_TXE  (1 << 7)
// Receive data register not empty (data ready to be read)
#define USART_ISR_RXNE (1 << 5)


static __always_inline void usart2_init(void)
{
    rcc_enable_usart2();

    gpio_moder_set(
        GPIOA_MODER,
        PA2,
        GPIO_MODE_ALT_FUNC
    );

    gpio_moder_set(
        GPIOA_MODER,
        PA3,
        GPIO_MODE_ALT_FUNC
    );

    gpio_afr_set(GPIOA_AFRL, PA2, AF1);
    gpio_afr_set(GPIOA_AFRL, PA3, AF1);

    // Tune the USART2
    USART2_BRR = 69;

    USART2_CR1 =
        USART_CR1_TE |
        USART_CR1_RE |
        USART_CR1_UE;
}

static __always_inline void usart2_write_byte(uint8_t byte)
{
    while (!(USART2_ISR & USART_ISR_TXE))
    {
    }

    USART2_TDR = byte;
}

static __always_inline uint8_t usart2_read_byte(void)
{
    while (!(USART2_ISR & USART_ISR_RXNE))
    {
    }

    return (uint8_t)USART2_RDR;
}

static __always_inline void usart2_write(
    const uint8_t *buf,
    uint32_t count)
{
    for (uint32_t i = 0; i < count; i++)
    {
        usart2_write_byte(buf[i]);
    }
}

static __always_inline void usart2_read(
    uint8_t *buf,
    uint32_t count)
{
    for (uint32_t i = 0; i < count; i++)
    {
        buf[i] = usart2_read_byte();
    }
}

#endif // UART_H

