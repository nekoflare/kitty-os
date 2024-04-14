//
// Created by Piotr on 14.04.2024.
//

#include "framebuffer.h"

static limine_framebuffer_request fb_request = {
        .id = LIMINE_FRAMEBUFFER_REQUEST,
        .revision = 0,
        .response = nullptr
};

limine_framebuffer* KeGetFramebuffer(std::size_t fb_idx)
{
    if (fb_request.response == nullptr)
    {
        return nullptr;
    }

    if (fb_request.response->framebuffer_count <= fb_idx)
    {
        return nullptr;
    }

    return fb_request.response->framebuffers[fb_idx];
}