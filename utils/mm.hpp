#ifndef MM_HPP
#define MM_HPP

#include <kern_api/kern_api.hpp>
#include <limine.h>

void print_virt_addr_as_str(void* addr);

namespace Memory
{
    void Init();
    void PrintMemoryEntries();

    enum PageAttributes
    {
        PRESENT = 1 << 0,
        READ_WRITE = 1 << 1,
        SUPERVISOR = 1 << 2,
        WRITE_THROUGH = 1 << 3,
        CACHE_DISABLE = 1 << 4,
        ACCESSED = 1 << 5,
        DIRTY = 1 << 6,
        PAGE_ATTRIBUTE_TABLE = 1 << 7,
        GLOBAL = 1 << 8,
    };
}

#endif