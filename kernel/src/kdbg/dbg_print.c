#include "dbg_print.h"

void debug_putc(const char c)
{
    outb(DEBUG_PORT, c);
}

void debug_print(const char* s)
{
    unsigned int idx = 0;
    while (s[idx] != 0)
    {
        debug_putc(s[idx]);
        idx++;
    }
}