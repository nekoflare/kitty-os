//
// Created by Piotr on 23.03.2024.
//

#include <limine.h>
#include "structures.h"

namespace Kernel
{
    struct limine_framebuffer_request limine_framebuffer_request = {
            .id = LIMINE_FRAMEBUFFER_REQUEST,
            .revision = 0,
            .response = nullptr
    };

    std::size_t get_framebuffer_count()
    {
        if (limine_framebuffer_request.response == nullptr)
        {
            return 0;
        }

        return limine_framebuffer_request.response->framebuffer_count;
    }

    limine_framebuffer** get_framebuffers()
    {
        if (limine_framebuffer_request.response == nullptr)
        {
            return nullptr;
        }

        return limine_framebuffer_request.response->framebuffers;
    }
}
