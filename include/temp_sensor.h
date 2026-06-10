
#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include "registers.h"
#include "tim7_timer.h"
#include "systick.h"

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

// Return:
// 1 -> sensor detected
// 0 -> no sensor
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


static __always_inline void ds18b20_write_bit(uint8_t bit)
{
    ds18b20_pin_output();

    if (bit)
    {
        // logical 1
        gpio_reset(GPIOA_BSRR, DS18B20_PIN);
        delay_us(6);                 // hold line for short time
        ds18b20_pin_input();         // release
        delay_us(64);                // the rest of the rime slot
    }
    else
    {
        // logical 0
        gpio_reset(GPIOA_BSRR, DS18B20_PIN);
        delay_us(60);                // hold line for long
        ds18b20_pin_input();
        delay_us(10);
    }
}

static __always_inline uint8_t ds18b20_read_bit(void)
{
    uint8_t bit;

    ds18b20_pin_output();

    gpio_reset(GPIOA_BSRR, DS18B20_PIN);
    delay_us(2);                     // start slot
    ds18b20_pin_input();             // release line

    delay_us(8);                     // wait for the sampling

    bit = gpio_read(GPIOA_IDR, DS18B20_PIN);

    delay_us(50);                    // end of slot

    return bit;
}

static __always_inline void ds18b20_write_byte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        ds18b20_write_bit(data & 1);
        data >>= 1;
    }
}

static __always_inline uint8_t ds18b20_read_byte(void)
{
    uint8_t result = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        result >>= 1;
        if (ds18b20_read_bit())
            result |= 0x80;
    }

    return result;
}



// The main func to check temp
static __always_inline __must_check int16_t
ds18b20_read_temperature(void)
{
    if (!ds18b20_reset())
        return 0x7FFF;        // error

    ds18b20_write_byte(0xCC); // Skip ROM
    ds18b20_write_byte(0x44); // Convert T

    // wait conversion (12-bit max ~750ms)
    delay_ms(750);

    ds18b20_reset();

    ds18b20_write_byte(0xCC);
    ds18b20_write_byte(0xBE); // Read scratchpad

    uint8_t lsb = ds18b20_read_byte();
    uint8_t msb = ds18b20_read_byte();

    int16_t temp = (msb << 8) | lsb;

    return temp >> 4;
}

#endif


