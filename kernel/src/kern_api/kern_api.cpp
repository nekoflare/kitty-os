#include <kern_api/kern_api.hpp>

volatile limine_framebuffer_request ke_fb_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = nullptr
};

bool KeIsFramebufferAvailable()
{
    if (ke_fb_request.response == nullptr)
    {
        return false;
    }

    return true;
}

u64 KeGetFramebufferCount()
{
    if (!KeIsFramebufferAvailable())
    {
        return 0;
    }

    return ke_fb_request.response->framebuffer_count;
}

limine_framebuffer* KeGetFramebuffer(u64 framebuffer_id)
{
    // Guard 1
    if (!KeIsFramebufferAvailable())
    {
        return nullptr;
    }

    // Guard 2
    if (framebuffer_id >= ke_fb_request.response->framebuffer_count)
    {
        return nullptr;
    }

    return ke_fb_request.response->framebuffers[framebuffer_id];
}

limine_framebuffer_response* KeGetFramebufferResponse()
{
    return ke_fb_request.response;
}