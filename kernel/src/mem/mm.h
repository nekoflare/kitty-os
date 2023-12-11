#pragma once

#include <limine.h>
#include <stddef.h>

struct KernelMemoryInformation
{
    unsigned long long memory_overall;
    unsigned long long usable_memory;
    unsigned long long unusable_memory;
};

extern struct limine_memmap_request mmap_request;

const char* type_to_string(
    unsigned int type
);

void initialize_mm(void);
struct KernelMemoryInformation* get_kmi(void);