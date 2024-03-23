//
// Created by Piotr on 23.03.2024.
//

#ifndef KITTY_OS_CPP_QDRAW_H
#define KITTY_OS_CPP_QDRAW_H

#include <cstdint>
#include <kernel/structures.h>

namespace Graphics
{
    typedef struct Color
    {
        std::uint8_t r, g, b;
    } color_t;
};

class QDraw
{
public:
    QDraw();

    void DrawPixel(int x, int y, Graphics::color_t color);
    void DrawLine(int x1, int y1, int x2, int y2, Graphics::color_t color);
    void DrawRectangle(int x, int y, int w, int h, int thickness, Graphics::color_t color);
    void DrawFilledRectangle(int x, int y, int w, int h, Graphics::color_t color);

private:
    limine_framebuffer** framebuffers;
    volatile void* fb;

    std::size_t fb_width;
    std::size_t fb_height;
    std::size_t fb_bpp;

    std::size_t pitch, red_mask_offset, green_mask_offset, blue_mask_offset;
    std::size_t red_mask_size, green_mask_size, blue_mask_size;
};


#endif //KITTY_OS_CPP_QDRAW_H
