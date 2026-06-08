#include <stdint.h>

#include "../include/registers.h"
#include "../include/uart.h"
#include "../include/systick.h"
#include "../include/sin_cos_table.h"

#define WORLD_SIZE 255

#define SCREEN_W 80
#define SCREEN_H 21

#define FOV 80
#define HALF_FOV (FOV / 2)

#define PROJECTION_SCALE 100

typedef int32_t fixed_t;

struct Player
{
    fixed_t x;
    fixed_t y;
    int16_t angle;
};

static uint8_t frame[SCREEN_W];

// Got this from internet, replace of multiply. Some how
// Cortex-M0 can't divide and multiply integers >:(
static int32_t soft_mul(int32_t a, int32_t b)
{
    int sign = 0;

    if (a < 0)
    {
        a = -a;
        sign ^= 1;
    }

    if (b < 0)
    {
        b = -b;
        sign ^= 1;
    }

    int32_t result = 0;

    while (b)
    {
        if (b & 1)
        {
            result += a;
        }

        a <<= 1;
        b >>= 1;
    }

    return sign ? -result : result;
}

static int32_t soft_div(int32_t dividend, int32_t divisor)
{
    if (divisor == 0)
    {
        return 0x7FFFFFFF;
    }

    int sign = 0;

    if (dividend < 0)
    {
        dividend = -dividend;
        sign ^= 1;
    }

    if (divisor < 0)
    {
        divisor = -divisor;
        sign ^= 1;
    }

    uint32_t quotient = 0;
    uint32_t remainder = 0;

    for (int i = 31; i >= 0; i--)
    {
        remainder <<= 1;
        remainder |= (dividend >> i) & 1;

        if (remainder >= (uint32_t)divisor)
        {
            remainder -= divisor;
            quotient |= (1u << i);
        }
    }

    return sign ? -(int32_t)quotient
                : (int32_t)quotient;
}


static int16_t normalize_angle(int16_t angle)
{
    while (angle < 0)
    {
        angle += 360;
    }

    while (angle >= 360)
    {
        angle -= 360;
    }

    return angle;
}

static fixed_t cast_ray(
    fixed_t x,
    fixed_t y,
    int16_t angle)
{
    int32_t dx = cos_table[angle];
    int32_t dy = sin_table[angle];

    int32_t tx = 0x7FFFFFFF;
    int32_t ty = 0x7FFFFFFF;

    if (dx > 0)
    {
        tx = soft_div(
            soft_mul(((WORLD_SIZE << 8) - x), TRIG_SCALE),
            dx
        );
    }
    else if (dx < 0)
    {
        tx = soft_div(
            soft_mul((0 - x), TRIG_SCALE),
            dx
        );
    }

    if (dy > 0)
    {
        ty = soft_div(
            soft_mul(((WORLD_SIZE << 8) - y), TRIG_SCALE),
            dy
        );
    }
    else if (dy < 0)
    {
        ty = soft_div(
            soft_mul((0 - y), TRIG_SCALE),
            dy
        );
    }

    return (tx < ty) ? tx : ty;
}

static void build_frame(const struct Player *player)
{
    for (int col = 0; col < SCREEN_W; col++)
    {
        int16_t ray_angle =
            normalize_angle(
                player->angle
                - HALF_FOV
                + col);

        fixed_t distance =
            cast_ray(
                player->x,
                player->y,
                ray_angle);

        int16_t angle_diff =
            normalize_angle(
                ray_angle - player->angle);

        if (angle_diff > 180)
        {
            angle_diff -= 360;
        }

        int32_t correction =
            cos_table[normalize_angle(angle_diff)];

        fixed_t corrected_distance =
            (distance * correction) >> 8;

        if (corrected_distance <= 0)
        {
            corrected_distance = 1;
        }

        int32_t height = soft_div(
            (SCREEN_H * PROJECTION_SCALE << 8),
            corrected_distance
        );

        if (height < 1)
        {
            height = 1;
        }

        if (height > SCREEN_H)
        {
            height = SCREEN_H;
        }

        frame[col] = (uint8_t)height;
    }
}

static void send_frame(void)
{
    usart2_write_byte(0xAA);

    usart2_write(
        frame,
        SCREEN_W);

    usart2_write_byte(0x55);
}

int main(void)
{
    rcc_enable_gpioa();

    systick_init();
    usart2_init();

    struct Player player =
    {
        .x = (64 << 8),
        .y = (128 << 8),
        .angle = 0
    };

    while (1)
    {
        uint8_t cmd =
            usart2_read_byte();

        if (cmd == 'a')
        {
            player.angle =
                normalize_angle(
                    player.angle + 1);
        }

        if (cmd == 'd')
        {
            player.angle =
                normalize_angle(
                    player.angle - 1);
        }

        build_frame(&player);

        send_frame();
    }

    return 0;
}
