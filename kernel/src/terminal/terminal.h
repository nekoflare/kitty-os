//
// Created by Piotr on 16.03.2024.
//

#ifndef KITTY_OS_CPP_TERMINAL_H
#define KITTY_OS_CPP_TERMINAL_H

#include <cstdint>

#define PSF_FONT_MAGIC 0x864ab572

typedef struct _psf_header
{
    std::uint32_t magic;
    std::uint32_t version;
    std::uint32_t header_size;
    std::uint32_t flags;
    std::uint32_t num_glyphs;
    std::uint32_t bytes_per_glyph;
    std::uint32_t height;
    std::uint32_t width;
} psf_header;

class Terminal {
public:
    Terminal(const char* font_buffer, std::uint64_t font_buffer_length);
    void RawPutChar(std::uint16_t c, int cx, int cy, uint32_t fg = 0xffffff, uint32_t bg = 0x000000);
private:
    const char* g_FontData;
    psf_header* g_PSFHeader;
    std::uint64_t g_GlyphWidth;
    std::uint64_t g_GlyphHeight;

    bool g_UsingUnicode;

    char* s;
};


#endif //KITTY_OS_CPP_TERMINAL_H
