#include <hal/processor/access.hpp>

namespace Processor
{
    uint64_t rdfsbase() {
        uint64_t fsbase;
        asm volatile("rdfsbase %0" : "=r" (fsbase));
        return fsbase;
    }

    uint64_t rdgsbase() {
        uint64_t gsbase;
        asm volatile("rdgsbase %0" : "=r" (gsbase));
        return gsbase;
    }

    void wrfsbase(uint64_t value) {
        asm volatile("wrfsbase %0" : : "r" (value));
    }

    void wrgsbase(uint64_t value) {
        asm volatile("wrgsbase %0" : : "r" (value));
    }

    uint64_t rdtsc() {
        uint32_t low, high;
        __asm__ volatile ("rdtsc" : "=a" (low), "=d" (high));
        return ((uint64_t)high << 32) | low;
    }

    void swapgs()
    {
        asm volatile ("swapgs");
    }  
}
