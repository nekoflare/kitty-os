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
#include <arch/x86/gdt/gdt.h>

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

    QDraw system_renderer;

    ktl::printf("Build date: " __DATE__ " " __TIME__ "\n");

    HAL::x86_init_gdt();

    x86_halt_system();
}