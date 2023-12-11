#include "psf2.h"
#include "klibc/string.h"
#include "io/serial.h"
#include "bgraphics/graphics.h"
#include <stddef.h>

char* psf2_font_data = NULL;
struct PSF2Header* psf2_header = NULL;

void initialize_psf2_font(
    char* psf2_font_address
)
{
    // Reinterpret the address as structure and check for the magic.
    // If magic isn't equal to the PSF2 magic then set psf2_header to nothing.

    struct PSF2Header* psf2_header_ptr = (struct PSF2Header*) psf2_font_address;

    if (psf2_header_ptr->magic != 0x864ab572)
    {
        serial_printf("PSF2 doesn't have valid magic.\n");
        serial_printf("The magic found: %x\n", psf2_header_ptr->magic);
        return;
    }

    psf2_header = psf2_header_ptr;
    psf2_font_data = psf2_font_address;
}

void draw_character_psf2(unsigned int x, unsigned int y, char character, unsigned char r, unsigned char g, unsigned char b, unsigned int sca, unsigned int scb)
{
    if (!psf2_header || !psf2_font_data) {
        serial_printf("PSF2 font data not initialized.\n");
        return;
    }

    // Ensure the character is within the range of available glyphs
    if (character < 0 || character >= 512) {
        serial_printf("Character out of range.\n");
        return;
    }

    unsigned int glyphSize = psf2_header->glyph_size;
    unsigned int glyphOffset = psf2_header->header_size + (character * glyphSize);
    unsigned char* glyphData = (unsigned char*)(psf2_font_data + glyphOffset);
    unsigned int glyphWidth = psf2_header->width;
    unsigned int glyphHeight = psf2_header->height;

    // Calculate scaled glyph dimensions based on user-defined scaling factors sca and scb
    unsigned int scaled_glyphWidth = (glyphWidth * sca) / scb;
    unsigned int scaled_glyphHeight = (glyphHeight * sca) / scb;

    // Iterate through the original glyph data to draw scaled pixels
    for (unsigned int dy = 0; dy < scaled_glyphHeight; dy++) {
        for (unsigned int dx = 0; dx < scaled_glyphWidth; dx++) {
            // Calculate corresponding position in the original glyph
            unsigned int original_dx = (dx * glyphWidth) / scaled_glyphWidth;
            unsigned int original_dy = (dy * glyphHeight) / scaled_glyphHeight;

            // Extract the bit corresponding to the current pixel from the original glyph data
            unsigned char pixelByte = glyphData[original_dy * ((glyphWidth + 7) / 8) + (original_dx / 8)];
            unsigned char mask = 1 << (7 - (original_dx % 8)); // Start from the leftmost bit

            // Check if the bit for this pixel is set
            if (pixelByte & mask) {
                // Draw the pixel at the corresponding position on the screen
                put_pixel(x + dx, y + dy, r, g, b);
            }
        }
    }
}


/*

void draw_character(unsigned int x, unsigned int y, char character, unsigned char r, unsigned char g, unsigned char b, int sca, int scb) {
    


*/