//
// Created by Piotr on 23.03.2024.
//

#ifndef KITTY_OS_CPP_STRUCTURES_H
#define KITTY_OS_CPP_STRUCTURES_H

#include <limine.h>
#include <cstdint>

namespace Kernel
{
    std::size_t get_framebuffer_count();
    limine_framebuffer** get_framebuffers();
}

#endif //KITTY_OS_CPP_STRUCTURES_H
