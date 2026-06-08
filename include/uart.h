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
#define USART_CR1_UE     (1 << 0)
// CR1 Receiver Enable (RX)
#define USART_CR1_RE     (1 << 2)
// CR1 transmitter enable (TX)
#define USART_CR1_TE     (1 << 3)
// CR1 Enable RXNE Interrupt
#define USART_CR1_RXNEIE (1 << 5)

/* ISR bits */

// Transmit data register empty (data ready to write)
#define USART_ISR_TXE    (1 << 7)
// Receive data register not empty (data ready to be read)
#define USART_ISR_RXNE   (1 << 5)

extern volatile uint8_t rx_buffer[64];
extern volatile uint8_t rx_head;
extern volatile uint8_t rx_tail;

// Interruption
void USART2_Handler(void);

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
        USART_CR1_RXNEIE |
        USART_CR1_UE;

    nvic_enable_usart2();
}

static __always_inline uint8_t usart2_has_data(void)
{
    return rx_head != rx_tail;
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
    uint8_t byte = rx_buffer[rx_tail];

    rx_tail = (rx_tail + 1) % 64;

    return byte;
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

static __always_inline void usart2_read(uint8_t *buf)
{
    uint8_t i = 0;
    while (rx_tail != rx_head)
    {
        buf[i] = rx_buffer[rx_tail];

        rx_tail = (rx_tail + 1) % 64;
        i++;
    }
}

#endif // UART_H

