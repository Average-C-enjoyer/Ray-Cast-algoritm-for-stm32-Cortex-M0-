
#include "../include/temp_sensor.h"
#include "../include/uart.h"
#include "../include/tim7_timer.h"

int main(void)
{
    rcc_enable_gpioa();

    systick_init();
    usart2_init();
    timer17_init();

    while (1)
    {
        if (ds18b20_reset())
        {
            int16_t temp = ds18b20_read_temperature();
            usart2_write((uint8_t*)&temp, 2);
        }
        else
        {
            usart2_write(
                (uint8_t*)"SENSOR NOT FOUND\r\n",
                11
            );
        }

        delay_ms(1000);
    }
}


