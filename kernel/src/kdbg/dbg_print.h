#pragma once

#include "io/io.h"

#define DEBUG_PORT 0xE9

void debug_putc(
    const char c
);

void debug_print(
    const char* s
);