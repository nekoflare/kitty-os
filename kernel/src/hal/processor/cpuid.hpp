#ifndef PROCESSOR_CPUID_HPP
#define PROCESSOR_CPUID_HPP

#include <cstdint>

namespace Processor
{
    void CpuID(std::uint32_t code, std::uint32_t& a, std::uint32_t& b, std::uint32_t& c, std::uint32_t& d);
};

#endif