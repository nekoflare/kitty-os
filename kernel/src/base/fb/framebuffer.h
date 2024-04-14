//
// Created by Piotr on 14.04.2024.
//

#ifndef KITTY_OS_CPP_FRAMEBUFFER_H
#define KITTY_OS_CPP_FRAMEBUFFER_H

#include <limine.h>
#include <cstdint>

limine_framebuffer* KeGetFramebuffer(std::size_t fb_idx);

#endif //KITTY_OS_CPP_FRAMEBUFFER_H
