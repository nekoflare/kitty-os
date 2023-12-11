#pragma once

#include <stdint.h>

static int abs(int v)
{
    if (0 > v)
    {
        return -v;
    }

    return v;
}