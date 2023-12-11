#ifndef HAL_H
#define HAL_H

#include <stdint.h>

void io_wait(void);
uint64_t rdtsc(void);
void cpuid(int code, uint32_t* a, uint32_t* d);
void initialize_hal(void);

#endif