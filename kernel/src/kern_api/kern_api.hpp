#ifndef KERN_API
#define KERN_API

#include <limine.h>
#include <types.h>

extern volatile limine_framebuffer_request ke_fb_request;

bool KeIsFramebufferAvailable();
u64 KeGetFramebufferCount();
limine_framebuffer* KeGetFramebuffer(u64 framebuffer_id);
limine_framebuffer_response* KeGetFramebufferResponse();

#endif