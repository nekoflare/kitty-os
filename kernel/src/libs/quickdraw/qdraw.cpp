//
// Created by Piotr on 23.03.2024.
//

#include <arch/x86/routines.h>
#include <libs/math/math.h>
#include "qdraw.h"

QDraw::QDraw()
{
    std::size_t fb_count = Kernel::get_framebuffer_count();
    limine_framebuffer** fb_structures = Kernel::get_framebuffers();

    if (0 >= fb_count)
    {
        x86_halt_system();
    }

    this->fb = reinterpret_cast<void*>(fb_structures[0]->address);
    this->fb_width = fb_structures[0]->width;
    this->fb_height = fb_structures[0]->height;
    this->fb_bpp = fb_structures[0]->bpp;
    this->framebuffers = fb_structures;
    this->pitch = fb_structures[0]->pitch;
    this->red_mask_offset = fb_structures[0]->red_mask_shift;
    this->red_mask_size =  fb_structures[0]->red_mask_size;
    this->green_mask_offset = fb_structures[0]->green_mask_shift;
    this->green_mask_size = fb_structures[0]->green_mask_size;
    this->blue_mask_offset = fb_structures[0]->blue_mask_shift;
    this->blue_mask_size = fb_structures[0]->blue_mask_size;
}

void QDraw::DrawPixel(int x, int y, Graphics::color_t color)
{
    if (static_cast<std::uint64_t>(x) > this->framebuffers[0]->width || static_cast<std::uint64_t>(y) > this->framebuffers[0]->height)
    {
        return;
    }

    std::uint8_t* fb_ptr = (std::uint8_t*) this->fb;

    // Calculate the offset to the pixel in the framebuffer
    std::size_t offset = y * pitch + (x * fb_bpp) / 8;

    // Determine the byte-aligned offsets for each color component
    std::size_t redOffset = this->red_mask_offset / 8;
    std::size_t greenOffset = this->green_mask_offset / 8;
    std::size_t blueOffset = this->blue_mask_offset / 8;

    // Calculate the bit offsets within each byte for each color component
    int redBitOffset = this->red_mask_offset % 8;
    int greenBitOffset = this->green_mask_offset % 8;
    int blueBitOffset = this->blue_mask_offset % 8;

    // Calculate the masks for each color component
    std::uint8_t redMask = (1 << this->red_mask_size) - 1;
    std::uint8_t greenMask = (1 << this->green_mask_size) - 1;
    std::uint8_t blueMask = (1 << this->blue_mask_size) - 1;

    // Pack the color components into the framebuffer memory
    fb_ptr[offset + redOffset] = (fb_ptr[offset + redOffset] & ~(redMask << redBitOffset)) |
                                 ((color.r & redMask) << redBitOffset);
    fb_ptr[offset + greenOffset] = (fb_ptr[offset + greenOffset] & ~(greenMask << greenBitOffset)) |
                                   ((color.g & greenMask) << greenBitOffset);
    fb_ptr[offset + blueOffset] = (fb_ptr[offset + blueOffset] & ~(blueMask << blueBitOffset)) |
                                  ((color.b & blueMask) << blueBitOffset);
}

void QDraw::DrawLine(int x1, int y1, int x2, int y2, Graphics::color_t color)
{
    // Calculate the differences between the endpoints
    int dx = ktl::abs(x2 - x1);
    int dy = ktl::abs(y2 - y1);

    // Determine the direction of the line
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    // Calculate the initial decision parameter
    int decision = (dx > dy ? dx : -dy) / 2;

    // Loop through all pixels along the line
    while (x1 != x2 || y1 != y2)
    {
        // Draw the pixel at the current position
        DrawPixel(x1, y1, color);

        // Update the decision parameter and move to the next pixel
        int e2 = decision;
        if (e2 > -dx)
        {
            decision -= dy;
            x1 += sx;
        }
        if (e2 < dy)
        {
            decision += dx;
            y1 += sy;
        }
    }

    // Draw the last pixel at the endpoint
    DrawPixel(x2, y2, color);
}

void QDraw::DrawRectangle(int x, int y, int w, int h, int thickness, Graphics::color_t color)
{
    // Draw horizontal lines (top and bottom sides)
    for (int i = 0; i < thickness; ++i)
    {
        for (int j = x; j < x + w; ++j)
        {
            DrawPixel(j, y + i, color);                // Top horizontal line
            DrawPixel(j, y + h - i - 1, color);        // Bottom horizontal line
        }
    }

    // Draw vertical lines (left and right sides)
    for (int i = 0; i < thickness; ++i)
    {
        for (int j = y + thickness; j < y + h - thickness; ++j)
        {
            DrawPixel(x + i, j, color);                // Left vertical line
            DrawPixel(x + w - i - 1, j, color);        // Right vertical line
        }
    }
}

void QDraw::DrawFilledRectangle(int x, int y, int w, int h, Graphics::color_t color)
{
    // Draw each pixel within the rectangle area
    for (int i = x; i < x + w; ++i)
    {
        for (int j = y; j < y + h; ++j)
        {
            DrawPixel(i, j, color);
        }
    }
}
