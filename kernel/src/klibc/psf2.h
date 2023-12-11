#pragma once

#include <stdint.h>

struct PSF2Header
{
    uint32_t magic;  // Always 72 b5 4a 86 or whatever that means.
    uint32_t version;
    uint32_t header_size;
    uint32_t flags;
    uint32_t length;
    uint32_t glyph_size;
    uint32_t height;
    uint32_t width;
} __attribute__((packed));

struct PSF2Header* psf2_header;

void initialize_psf2_font(
    char* psf2_font_address
);

void draw_character_psf2(unsigned int x, unsigned int y, char character, unsigned char r, unsigned char g, unsigned char b, unsigned int sca, unsigned int scb);