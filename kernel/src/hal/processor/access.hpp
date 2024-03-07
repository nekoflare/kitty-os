#ifndef PROCESSOR_ACCESS_HPP
#define PROCESSOR_ACCESS_HPP

#include <cstdint>

// Define the rdmsr function to read from a Model Specific Register (MSR)
inline uint64_t rdmsr(uint32_t msr) {
    uint32_t low, high;
    asm volatile("rdmsr" : "=a" (low), "=d" (high) : "c" (msr));
    return ((uint64_t)high << 32) | low;
}

// Define the wrmsr function to write to a Model Specific Register (MSR)
inline void wrmsr(uint32_t msr, uint64_t value) {
    uint32_t low = (uint32_t)value;
    uint32_t high = (uint32_t)(value >> 32);
    asm volatile("wrmsr" : : "c" (msr), "a" (low), "d" (high));
}

namespace Processor {
    // Read control register CRx
    #define READ_CR(reg) \
        inline std::uint64_t GetCR##reg() { \
            std::uint64_t cr##reg; \
            asm volatile ("movq %%cr" #reg ", %0" : "=r"(cr##reg)); \
            return cr##reg; \
        }

    // Write control register CRx
    #define WRITE_CR(reg) \
        inline void SetCR##reg(std::uint64_t cr##reg) { \
            asm volatile ("movq %0, %%cr" #reg : : "r"(cr##reg)); \
        }

    // Generate macros for CR0 to CR15
    READ_CR(0)
    WRITE_CR(0)
    READ_CR(1)
    WRITE_CR(1)
    READ_CR(2)
    WRITE_CR(2)
    READ_CR(3)
    WRITE_CR(3)
    READ_CR(4)
    WRITE_CR(4)
    READ_CR(5)
    WRITE_CR(5)
    READ_CR(6)
    WRITE_CR(6)
    READ_CR(7)
    WRITE_CR(7)
    READ_CR(8)
    WRITE_CR(8)
    READ_CR(9)
    WRITE_CR(9)
    READ_CR(10)
    WRITE_CR(10)
    READ_CR(11)
    WRITE_CR(11)
    READ_CR(12)
    WRITE_CR(12)
    READ_CR(13)
    WRITE_CR(13)
    READ_CR(14)
    WRITE_CR(14)
    READ_CR(15)
    WRITE_CR(15)

    uint64_t rdtsc();

    uint64_t rdfsbase();
    uint64_t rdgsbase();
    void wrfsbase(uint64_t value);
    void wrgsbase(uint64_t value);
    void swapgs();
}

#endif
