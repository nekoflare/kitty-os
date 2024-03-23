//
// Created by Piotr on 23.03.2024.
//

#include <cstdint>
#include <libs/flanterm/backends/fb.h>
#include <libs/flanterm/flanterm.h>
#include <kernel/structures.h>
#include <libs/kstd/string.h>
#include "stdio.h"


namespace ktl
{
    flanterm_context* ft_ctx;

    void init_stdio()
    {
        limine_framebuffer** fbs = Kernel::get_framebuffers();
        limine_framebuffer* curr_fb = fbs[0];

        ft_ctx = flanterm_fb_simple_init(reinterpret_cast<std::uint32_t*>(curr_fb->address), curr_fb->width, curr_fb->height, curr_fb->pitch, curr_fb->red_mask_size, curr_fb->red_mask_shift, curr_fb->green_mask_size, curr_fb->green_mask_shift, curr_fb->blue_mask_size, curr_fb->blue_mask_shift);
    }

    void putc(char c, Graphics::color_t bg, Graphics::color_t fg)
    {
        ft_ctx->set_text_bg_rgb(ft_ctx, bg.r << 16 | bg.g << 8 | bg.b);
        ft_ctx->set_text_fg_rgb(ft_ctx, fg.r << 16 | fg.g << 8 | fg.b);
        flanterm_write(ft_ctx, reinterpret_cast<const char*>(&c), 1);
    }

    void puts(const char* s, Graphics::color_t bg, Graphics::color_t fg)
    {
        ft_ctx->set_text_bg_rgb(ft_ctx, bg.r << 16 | bg.g << 8 | bg.b);
        ft_ctx->set_text_fg_rgb(ft_ctx, fg.r << 16 | fg.g << 8 | fg.b);
        flanterm_write(ft_ctx, s, strlen(s));
    }

    void printf(const char* format, ...)
    {
        va_list args;
        va_start(args, format);

        Graphics::color_t fg = {255, 255, 255};
        Graphics::color_t bg = {0, 0, 0};

        std::uint32_t ul;
        char hex[7] = {0}; // RRGGBB\0

        while (*format)
        {
            if (*format == '%')
            {
                ++format;
                switch (*format)
                {
                    default:
                        ktl::putc(*format, bg, fg);
                        ++format;
                        break;
                }
            }
            else if (*format == '\\')
            {
                ++format;
                switch (*format)
                {
                    case '[': // Escape codes
                        ++format;

                        // Escape code type
                        switch (*format)
                        {
                            case 'f': // switch foreground color
                                ++format;

                                // get the next 6 chars in RGB format.

                                hex[0] = *format;
                                ++format;
                                hex[1] = *format;
                                ++format;
                                hex[2] = *format;
                                ++format;
                                hex[3] = *format;
                                ++format;
                                hex[4] = *format;
                                ++format;
                                hex[5] = *format;
                                ++format;
                                hex[6] = 0; // null terminate it.

                                ul = strtoul(hex);

                                fg.r = ((*reinterpret_cast<std::uint32_t*>(&ul)) & 0xff0000) >> 16;
                                fg.g = ((*reinterpret_cast<std::uint32_t*>(&ul)) & 0x00ff00) >> 8;
                                fg.b = ((*reinterpret_cast<std::uint32_t*>(&ul)) & 0x0000ff);

                                break;
                            case 'b': // switch background color
                                ++format;

                                // get the next 6 chars in RGB format.

                                hex[0] = *format;
                                ++format;
                                hex[1] = *format;
                                ++format;
                                hex[2] = *format;
                                ++format;
                                hex[3] = *format;
                                ++format;
                                hex[4] = *format;
                                ++format;
                                hex[5] = *format;
                                ++format;
                                hex[6] = 0; // null terminate it.

                                ul = strtoul(hex);

                                bg.r = ((*reinterpret_cast<std::uint32_t*>(&ul)) & 0xff0000) >> 16;
                                bg.g = ((*reinterpret_cast<std::uint32_t*>(&ul)) & 0x00ff00) >> 8;
                                bg.b = ((*reinterpret_cast<std::uint32_t*>(&ul)) & 0x0000ff);

                                break;
                            default:
                                ktl::putc(*format, bg, fg);
                                ++format;
                                break;
                        }

                        break;
                    default:
                        ktl::putc(*format, bg, fg);
                        ++format;
                        break;
                }
            }
            else
            {
                ktl::putc(*format, bg, fg);
                ++format;
            }
        }

        va_end(args);
    }
}