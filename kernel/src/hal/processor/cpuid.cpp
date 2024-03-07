#include <hal/processor/cpuid.hpp>

void Processor::CpuID(std::uint32_t code, std::uint32_t& a, std::uint32_t& b, std::uint32_t& c, std::uint32_t& d)
{
    asm volatile (
        "cpuid;"
        : "=a" (a), "=b" (b), "=c" (c), "=d" (d)
        : "a" (code)
    );
}