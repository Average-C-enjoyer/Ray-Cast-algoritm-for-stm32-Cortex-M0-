#include "../include/temp_sensor.h"
#include "../include/uart.h"

int main(void)
{
    rcc_enable_gpioa();

    systick_init();
    usart2_init();

    while (1)
    {
        if (ds18b20_reset())
        {
            usart2_write(
                (uint8_t*)"FOUND\r\n",
                7
            );
        }
        else
        {
            usart2_write(
                (uint8_t*)"NOT FOUND\r\n",
                11
            );
        }

        delay_ms(1000);
    }
}

