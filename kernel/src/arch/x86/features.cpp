//
// Created by Piotr on 24.03.2024.
//

#include <libs/kstd/stdio.h>
#include <arch/x86/features.h>

namespace Kernel
{
    void print_cpu_features()
    {
        ktl::printf(CUTE_COLORS "CPU Features: \n");
        bool first_feature = true; // Track if it's the first feature

        if (has_sse())
        {
            if (!first_feature) ktl::printf(CUTE_COLORS ", "); // Append comma if not the first feature
            ktl::printf(CUTE_COLORS "SSE");
            first_feature = false; // Update flag
        }
        if (has_sse2())
        {
            if (!first_feature) ktl::printf(CUTE_COLORS ", ");
            ktl::printf(CUTE_COLORS "SSE2");
            first_feature = false;
        }
        if (has_sse3())
        {
            if (!first_feature) ktl::printf(CUTE_COLORS ", ");
            ktl::printf(CUTE_COLORS "SSE3");
            first_feature = false;
        }
        if (has_ssse3())
        {
            if (!first_feature) ktl::printf(CUTE_COLORS ", ");
            ktl::printf(CUTE_COLORS "SSSE3");
            first_feature = false;
        }
        if (has_sse4_1())
        {
            if (!first_feature) ktl::printf(CUTE_COLORS ", ");
            ktl::printf(CUTE_COLORS "SSE4.1");
            first_feature = false;
        }
        if (has_sse4_2())
        {
            if (!first_feature) ktl::printf(CUTE_COLORS ", ");
            ktl::printf(CUTE_COLORS "SSE4.2");
            first_feature = false;
        }
        if (has_sse4a())
        {
            if (!first_feature) ktl::printf(CUTE_COLORS ", ");
            ktl::printf(CUTE_COLORS "SSE4A");
            first_feature = false;
        }
        if (has_xop())
        {
            if (!first_feature) ktl::printf(CUTE_COLORS ", ");
            ktl::printf(CUTE_COLORS "XOP");
            first_feature = false;
        }
        if (has_fma4())
        {
            if (!first_feature) ktl::printf(CUTE_COLORS ", ");
            ktl::printf(CUTE_COLORS "FMA4");
            first_feature = false;
        }
        if (has_cvt16())
        {
            if (!first_feature) ktl::printf(CUTE_COLORS ", ");
            ktl::printf(CUTE_COLORS "CVT16");
            first_feature = false;
        }
        if (has_avx())
        {
            if (!first_feature) ktl::printf(CUTE_COLORS ", ");
            ktl::printf(CUTE_COLORS "AVX");
            first_feature = false;
        }
        if (has_xsave())
        {
            if (!first_feature) ktl::printf(CUTE_COLORS ", ");
            ktl::printf(CUTE_COLORS "XSAVE");
            first_feature = false;
        }
        if (has_avx2())
        {
            if (!first_feature) ktl::printf(CUTE_COLORS ", ");
            ktl::printf(CUTE_COLORS "AVX2");
            first_feature = false;
        }
        if (has_avx512f())
        {
            if (!first_feature) ktl::printf(CUTE_COLORS ", ");
            ktl::printf(CUTE_COLORS "AVX512F");
            first_feature = false;
        }

        ktl::printf(CUTE_COLORS "\n");
    }
}