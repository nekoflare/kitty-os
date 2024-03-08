#ifndef ERROR_CODES_HPP
#define ERROR_CODES_HPP

#include <stdint.h>

enum REGION : uint64_t
{
    ACPI,
    DRIVERS,
    FILESYSTEM,
    GRAPHICS,
    HAL,
    API,
    KLIBC,
    MEMORY,
    MULTITASKING,
    POWER,
    GENERIC
};

enum LEVEL : uint64_t
{
    OK,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    CRIT,
    ALERT
};

#define ERROR(region, level, id) (region << 48) | (level) | (id << 4)

enum ERROR_CODE : uint64_t
{
    // Generic errors
    SUCCESS = ERROR(GENERIC, OK, 0),
    FAIL = ERROR(GENERIC, WARN, 1),
    CRIT_FAIL = ERROR(GENERIC, CRIT, 2),
    VERY_CRIT_FAIL = ERROR(GENERIC, ALERT, 3),

    // Graphics errors
    NO_FRAMEBUFFERS = ERROR(GRAPHICS, CRIT, 0),
    FRAMEBUFFER_NULL = ERROR(GRAPHICS, CRIT, 1),
};

#endif