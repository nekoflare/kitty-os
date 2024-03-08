#include <graphics/graphics.hpp>
#include <kern_api/kern_api.hpp>
#include <error_codes.hpp>
#include <math/math.hpp>
#include <libc/stdio.hpp>

namespace Graphics
{
    uint64_t LastError = 0;
    limine_framebuffer** limine_framebuffers = nullptr;
    uint framebuffer_count = 0;
}

ERROR_CODE Graphics::QuickDraw::QDInit()
{
    Graphics::LastError = SUCCESS;

    // Check count of framebuffers.
    auto fb_count = KeGetFramebufferCount();
    if (0 >= fb_count)
    {
        return NO_FRAMEBUFFERS;
    }

    auto framebuffers = KeGetFramebufferResponse();
    Graphics::limine_framebuffers = framebuffers->framebuffers;
    Graphics::framebuffer_count = fb_count;

    if (framebuffers == nullptr)
    {
        return FRAMEBUFFER_NULL;
    }

    return SUCCESS;
}

static uint32_t calculate_color32(limine_framebuffer* this_framebuffer, char r, char g, char b)
{
    // Assuming this_framebuffer is a pointer to a structure with the required mask size and shift values.
    uint8_t red_mask_size = this_framebuffer->red_mask_size;
    uint8_t red_mask_shift = this_framebuffer->red_mask_shift;
    uint8_t green_mask_size = this_framebuffer->green_mask_size;
    uint8_t green_mask_shift = this_framebuffer->green_mask_shift;
    uint8_t blue_mask_size = this_framebuffer->blue_mask_size;
    uint8_t blue_mask_shift = this_framebuffer->blue_mask_shift;

    // Calculate the full 8-bit values for R, G, and B components
    uint8_t fullRed = (r << (8 - red_mask_size)) | (r >> red_mask_shift);
    uint8_t fullGreen = (g << (8 - green_mask_size)) | (g >> green_mask_shift);
    uint8_t fullBlue = (b << (8 - blue_mask_size)) | (b >> blue_mask_shift);

    // Combine the R, G, and B values to obtain the final color value
    uint32_t color = (fullRed << 16) | (fullGreen << 8) | fullBlue;

    return color;
}

void Graphics::QuickDraw::DrawPixel(uint screen_id, uint x, uint y, uchar r, uchar g, uchar b)
{
	// Check if screen_id is in boundaries.
    if (screen_id >= Graphics::framebuffer_count)
    {
        return;
    }

    // Check if X and Y is in boundaries.
    if (x >= Graphics::limine_framebuffers[screen_id]->width || y >= Graphics::limine_framebuffers[screen_id]->height)
    {
        return;
    }

    // Calculate the color value.
    uint32_t color = calculate_color32(Graphics::limine_framebuffers[screen_id], r, g, b);

    // Calculate the address of the pixel in the framebuffer.
    uint32_t pixel_address = y * Graphics::limine_framebuffers[screen_id]->pitch + x * 4; // Assuming 32-bit color (4 bytes per pixel)

    // Set the pixel color in the framebuffer.
    *reinterpret_cast<uint32_t*>(Graphics::limine_framebuffers[screen_id]->address + pixel_address) = color;

    // Optionally, you may need to flush or invalidate the framebuffer cache depending on your platform.
    // This ensures that the pixel value is written to the framebuffer immediately.z
    // For example, on some ARM platforms, you might use specific cache maintenance instructions.
}

void Graphics::QuickDraw::DrawRectangle(uint screen_id, uint x, uint y, uint w, uint h, uchar r, uchar g, uchar b)
{
    // Draw top horizontal line
    for (uint i = x; i < x + w; ++i)
        DrawPixel(screen_id, i, y, r, g, b);

    // Draw bottom horizontal line
    for (uint i = x; i < x + w; ++i)
        DrawPixel(screen_id, i, y + h - 1, r, g, b);

    // Draw left vertical line
    for (uint i = y; i < y + h; ++i)
        DrawPixel(screen_id, x, i, r, g, b);

    // Draw right vertical line
    for (uint i = y; i < y + h; ++i)
        DrawPixel(screen_id, x + w - 1, i, r, g, b);
}

void Graphics::QuickDraw::DrawFilledRectangle(uint screen_id, uint x, uint y, uint w, uint h, uchar r, uchar g, uchar b)
{
    // Iterate over each pixel within the rectangle's boundary
    for (uint i = x; i < x + w; ++i) {
        for (uint j = y; j < y + h; ++j) {
            // Draw the pixel at coordinates (i, j) with the specified color
            DrawPixel(screen_id, i, j, r, g, b);
        }
    }
}

void Graphics::QuickDraw::DrawLine(uint screen_id, uint x1, uint y1, uint x2, uint y2, uchar r, uchar g, uchar b)
{
    // Calculate differences and absolute differences between the two points
    int dx = Math::abs(static_cast<int>(x2) - static_cast<int>(x1));
    int dy = Math::abs(static_cast<int>(y2) - static_cast<int>(y1));
    int dx2 = 2 * dx; // Double the differences for efficiency
    int dy2 = 2 * dy;

    // Calculate the direction of movement for x and y
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    // Initialize error values
    int err = (dx > dy ? dx : -dy) / 2;
    int e2;

    while (true)
    {
        // Draw the current pixel
        DrawPixel(screen_id, x1, y1, r, g, b);

        // Check if we have reached the end point
        if (x1 == x2 && y1 == y2)
            break;

        // Calculate next point coordinates
        e2 = err;
        if (e2 > -dx)
        {
            err -= dy2;
            x1 += sx;
        }
        if (e2 < dy)
        {
            err += dx2;
            y1 += sy;
        }
    }
}

void Graphics::QuickDraw::put_bitmap_image_data(uint screen_id, char* bitmap_data, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    // Draw the BMP image using put_pixel
    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            unsigned char b = bitmap_data[3 * (i * width + j)];
            unsigned char g = bitmap_data[3 * (i * width + j) + 1];
            unsigned char r = bitmap_data[3 * (i * width + j) + 2];
            Graphics::QuickDraw::DrawPixel(screen_id, x + j, y + height - 1 - i, r, g, b);
        }
    }
}