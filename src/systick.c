#include "../include/systick.h"

volatile uint32_t ticks = 0;

void SysTick_Handler(void)
{
    ticks++;
}
