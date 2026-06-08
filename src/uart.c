#include "../include/uart.h"

volatile uint8_t rx_buffer[64] = {0};
volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;

void USART2_IRQHandler(void)
{
    uint8_t next_head =
    (rx_head + 1) % 64;

    if (next_head != rx_tail)
    {
        rx_buffer[rx_head] =
            (uint8_t)USART2_RDR;

        rx_head = next_head;
    }
    else
    {
        // overflow
        return;
    }
}

