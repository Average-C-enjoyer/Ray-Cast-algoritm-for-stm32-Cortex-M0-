#ifndef UTILS_H
#define UTILS_H

#define TRUE 1
#define FALSE 0

#define __must_check     __attribute__((warn_unused_result))
#define __noreturn       __attribute__((noreturn))
#define __always_inline  inline __attribute__((always_inline))
#define __packed         __attribute__((packed))

#define likely(x)        __builtin_expect(!!(x), 1)
#define unlikely(x)      __builtin_expect(!!(x), 0)

#endif // UTILS_H
