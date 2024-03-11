#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

enum KO_REASON : uint64_t
{
    NO_REASON = 0,
    BUG_CHECK_FAIL = 1,
};

typedef struct _ko_reason_pair
{
    uint64_t idx;
    const char* reason;
} ko_reason_pair;

#endif