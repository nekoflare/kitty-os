//
// Created by Piotr on 23.03.2024.
//

#include "routines.h"

extern "C" [[gnu::noreturn]] void x86_halt_system()
{
    asm volatile ("cli");
    asm volatile ("hlt");
}