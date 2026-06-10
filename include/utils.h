
#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define TRUE 1
#define FALSE 0

#define __must_check     __attribute__((warn_unused_result))
#define __noreturn       __attribute__((noreturn))
#define __always_inline  inline __attribute__((always_inline))
#define __packed         __attribute__((packed))

#define likely(x)        __builtin_expect(!!(x), 1)
#define unlikely(x)      __builtin_expect(!!(x), 0)

// Got this from internet, replace of multiply. Some how
// Cortex-M0 can't divide and multiply integers >:(
static __always_inline int32_t soft_mul(int32_t a, int32_t b)
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

static __always_inline int32_t soft_div(int32_t dividend, int32_t divisor)
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


#endif // UTILS_H

