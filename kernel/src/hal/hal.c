#include "hal.h"
#include "gdt.h"
#include "io/io.h"
#include <stdint.h>

void io_wait(void)
{
    outb(0x80, 0);
}

uint64_t rdtsc(void)
{
    uint32_t low, high;
    asm volatile("rdtsc":"=a"(low),"=d"(high));
    return ((uint64_t)high << 32) | low;
}

void cpuid(int code, uint32_t* a, uint32_t* d)
{
    asm volatile ( "cpuid" : "=a"(*a), "=d"(*d) : "0"(code) : "ebx", "ecx" );
}

void initialize_hal(void)
{
    initialize_gdt();
}