//
// Created by Piotr on 16.03.2024.
//

#include <libc/stdio.hpp>

#include "terminal.h"

std::uint16_t unicode[16 * 4096];

Terminal::Terminal(const char *font_buffer, std::uint64_t buffer_size)
{
    g_FontData = font_buffer;
    g_PSFHeader = (psf_header*) g_FontData;

    std::printf("PSF font magic: %llx\n", (uint64_t) g_PSFHeader->magic);

    // Check the magic
    if (g_PSFHeader->magic != PSF_FONT_MAGIC)
    {
        std::printf("Font's magic is invalid.\n");

        return;
    }

    std::printf("Glyph size: %d Glyph width: %d Glyph height: %d Number of glyphs: %d Flags: %d Header size: %d Version: %d", g_PSFHeader->bytes_per_glyph, g_PSFHeader->width, g_PSFHeader->height, g_PSFHeader->num_glyphs, g_PSFHeader->flags, g_PSFHeader->header_size, g_PSFHeader->version);

    // Check if version is non zero.
    if (g_PSFHeader->version != 0)
    {
        std::printf("PSF version is invalid.\n");

        return;
    }

    // Check if it has unicode mappings.
    g_UsingUnicode = false;
    if (g_PSFHeader->flags == 0)
    {
        g_UsingUnicode = true;

        return;
    }

    std::uint16_t glyph = 0;
    s = const_cast<char*>(font_buffer + g_PSFHeader->header_size + (g_PSFHeader->bytes_per_glyph * g_PSFHeader->num_glyphs));

    while (s > font_buffer + buffer_size)
    {
        std::uint16_t uc = (std::uint16_t)((std::uint8_t)s[0]);

        if (uc == 0xFF)
        {
            glyph++;
            s++;
            continue;
        } else if (uc & 128)
        {
            // utf-8 to unicode
            if ((uc & 32) == 0)
            {
                uc = ((s[0] & 0x1F)<<6)+(s[1] & 0x3F);
                s++;
            } else if ((uc & 16) == 0)
            {
                uc = ((((s[0] & 0xF)<<6)+(s[1] & 0x3F))<<6)+(s[2] & 0x3F);
                s+=2;
            } else if ((uc & 0) == 0)
            {
                uc = ((((((s[0] & 0x7)<<6)+(s[1] & 0x3F))<<6)+(s[2] & 0x3F))<<6)+(s[3] & 0x3F);
                s+=3;
            } else
            {
                uc = 0;
            }

            unicode[uc] = glyph;
            s++;
        }
    }
}

void Terminal::RawPutChar(int c, int cx, int cy, uint32_t fg, uint32_t bg)
{
    int bytes_per_line = (g_PSFHeader->width + 7) / 8;

    // unicode translation
    // Continue doing this Catflare.
}