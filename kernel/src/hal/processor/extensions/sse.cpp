#include <hal/processor/extensions/sse.hpp>
#include <hal/processor/extensions/bits.hpp>
#include <hal/processor/access.hpp>
#include <hal/processor/cpuid.hpp>

bool Processor::Extensions::HasSSE()
{
    std::uint32_t eax, ebx, ecx, edx;
    std::uint32_t code = SSE_CPUID_FUNCTION;
    
    Processor::CpuID(code, eax, ebx, ecx, edx);

    if (edx & SSE_CPUID_BIT)
    {
        return true;
    }

    return false;
}

bool Processor::Extensions::HasSSE2()
{
    std::uint32_t eax, ebx, ecx, edx;
    std::uint32_t code = SSE2_CPUID_FUNCTION;
    
    Processor::CpuID(code, eax, ebx, ecx, edx);

    if (edx & SSE4_2_CPUID_BIT)
    {
        return true;
    }

    return false;
}

bool Processor::Extensions::HasSSSE3()
{
    std::uint32_t eax, ebx, ecx, edx;
    std::uint32_t code = SSSE3_CPUID_FUNCTION;
    
    Processor::CpuID(code, eax, ebx, ecx, edx);

    if (ecx & SSSE3_CPUID_BIT)
    {
        return true;
    }

    return false;
}

bool Processor::Extensions::HasSSE4_1()
{
    std::uint32_t eax, ebx, ecx, edx;
    std::uint32_t code = SSE4_1_CPUID_FUNCTION;
    
    Processor::CpuID(code, eax, ebx, ecx, edx);

    if (ecx & SSE4_1_CPUID_BIT)
    {
        return true;
    }

    return false;
}

bool Processor::Extensions::HasSSE4_2()
{
    std::uint32_t eax, ebx, ecx, edx;
    std::uint32_t code = SSE4_2_CPUID_FUNCTION;
    
    Processor::CpuID(code, eax, ebx, ecx, edx);

    if (ecx & SSE4_2_CPUID_BIT)
    {
        return true;
    }

    return false;
}

bool Processor::Extensions::HasSSE4A()
{
    std::uint32_t eax, ebx, ecx, edx;
    std::uint32_t code = SSE4A_CPUID_FUNCTION;
    
    Processor::CpuID(code, eax, ebx, ecx, edx);

    if (ecx & SSE4A_CPUID_BIT)
    {
        return true;
    }

    return false;
}

void Processor::Extensions::EnableSSE()
{
    std::uint32_t cr0 = Processor::GetCR0();

    cr0 &= 0xFFFFFFFB;
    cr0 |= 0x2;

    Processor::SetCR0(cr0);

    std::uint32_t cr4 = Processor::GetCR4();

    cr4 |= (3 << 9);

    Processor::SetCR4(cr4);
}