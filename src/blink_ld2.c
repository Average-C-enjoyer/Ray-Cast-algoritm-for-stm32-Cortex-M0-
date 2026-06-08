#include "../include/registers.h"
#include "../include/systick.h"
#include "../include/uart.h"


int main(void)
{
    rcc_enable_gpioa();

    systick_init();
    usart2_init();

    gpio_moder_set(GPIOA_MODER, PA5, GPIO_MODE_OUTPUT);

    uint8_t buffer[5];
    while (1) {
        //Invert the PA5 pin value
        GPIOA_ODR ^= (1 << PA5);
        delay_ms(1000);

        // Just test the uart driver

        //usart2_write_byte('A');
        //usart2_write((uint8_t*)"Hello world!\n", 13);

        // Echo
        usart2_read(buffer, 5);
        usart2_write(buffer, 5);
    }
}

