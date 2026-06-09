#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include "registers.h"
#include "systick.h"
#include "tim7_timer.h"

#define DS18B20_PIN PA0

static __always_inline void ds18b20_pin_output(void)
{
    gpio_moder_set(
        GPIOA_MODER,
        DS18B20_PIN,
        GPIO_MODE_OUTPUT
    );
}

static __always_inline void ds18b20_pin_input(void)
{
    gpio_moder_set(
        GPIOA_MODER,
        DS18B20_PIN,
        GPIO_MODE_INPUT
    );
}

/*
 * Return:
 * 1 -> sensor detected
 * 0 -> no sensor
 */
static __always_inline uint8_t ds18b20_reset(void)
{
    ds18b20_pin_output();

    gpio_reset(
        GPIOA_BSRR,
        DS18B20_PIN
    );

    delay_us(480);

    ds18b20_pin_input();

    // If sensor is alive it will pull line to 0
    // in 15 to 60 us
    delay_us(70);

    uint8_t present =
        !gpio_read(
            GPIOA_IDR,
            DS18B20_PIN
        );

    delay_us(410);

    return present;
}

#endif

