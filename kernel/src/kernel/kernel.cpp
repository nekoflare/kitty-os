//
// Created by Piotr on 23.03.2024.
//

#include <limine.h>
#include <cstdint>
#include <arch/x86/routines.h>
#include <kernel/structures.h>
#include <libs/kstd/string.h>
#include <libs/quickdraw/qdraw.h>
#include <libs/kstd/stdio.h>
#include <arch/x86/features.h>

LIMINE_BASE_REVISION(1)

extern void (*__init_array[])();
extern void (*__init_array_end[])();

void main()
{
    if (LIMINE_BASE_REVISION_SUPPORTED == false)
    {
        x86_halt_system();
    }

    for (std::size_t i = 0; &__init_array[i] != __init_array_end; i++)
    {
        __init_array[i]();
    }

    ktl::init_stdio();

    QDraw this_renderer;
    this_renderer.DrawLine(500, 500, 200, 250, {255, 255, 255});
    this_renderer.DrawRectangle(500, 500, 100, 200, 5, {255, 0, 255});
    this_renderer.DrawFilledRectangle(600, 600, 200, 100, {255, 255, 0});

    ktl::printf("Build date: " __DATE__ " " __TIME__ "\n");
    ktl::printf(CUTE_COLORS "CPU Features: \n");
    bool first_feature = true; // Track if it's the first feature

    if (has_sse())
    {
        if (!first_feature) ktl::printf(", "); // Append comma if not the first feature
        ktl::printf(CUTE_COLORS "SSE");
        first_feature = false; // Update flag
    }
    if (has_sse2())
    {
        if (!first_feature) ktl::printf(", ");
        ktl::printf(CUTE_COLORS "SSE2");
        first_feature = false;
    }
    if (has_sse3())
    {
        if (!first_feature) ktl::printf(", ");
        ktl::printf(CUTE_COLORS "SSE3");
        first_feature = false;
    }
    if (has_ssse3())
    {
        if (!first_feature) ktl::printf(", ");
        ktl::printf(CUTE_COLORS "SSSE3");
        first_feature = false;
    }
    if (has_sse4_1())
    {
        if (!first_feature) ktl::printf(", ");
        ktl::printf(CUTE_COLORS "SSE4.1");
        first_feature = false;
    }
    if (has_sse4_2())
    {
        if (!first_feature) ktl::printf(", ");
        ktl::printf(CUTE_COLORS "SSE4.2");
        first_feature = false;
    }
    if (has_sse4a())
    {
        if (!first_feature) ktl::printf(", ");
        ktl::printf(CUTE_COLORS "SSE4A");
        first_feature = false;
    }
    if (has_xop())
    {
        if (!first_feature) ktl::printf(", ");
        ktl::printf(CUTE_COLORS "XOP");
        first_feature = false;
    }
    if (has_fma4())
    {
        if (!first_feature) ktl::printf(", ");
        ktl::printf(CUTE_COLORS "FMA4");
        first_feature = false;
    }
    if (has_cvt16())
    {
        if (!first_feature) ktl::printf(", ");
        ktl::printf(CUTE_COLORS "CVT16");
        first_feature = false;
    }
    if (has_avx())
    {
        if (!first_feature) ktl::printf(", ");
        ktl::printf(CUTE_COLORS "AVX");
        first_feature = false;
    }
    if (has_xsave())
    {
        if (!first_feature) ktl::printf(", ");
        ktl::printf(CUTE_COLORS "XSAVE");
        first_feature = false;
    }
    if (has_avx2())
    {
        if (!first_feature) ktl::printf(", ");
        ktl::printf(CUTE_COLORS "AVX2");
        first_feature = false;
    }
    if (has_avx512f())
    {
        if (!first_feature) ktl::printf(", ");
        ktl::printf(CUTE_COLORS "AVX512F");
        first_feature = false;
    }

    ktl::printf(CUTE_COLORS "\n");


    x86_halt_system();
}