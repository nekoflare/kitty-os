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

    ktl::printf(CUTE_COLORS "Hello, World!\n");

    ktl::printf(NRILL_COLORS "Hello, World!\n");

    x86_halt_system();
}