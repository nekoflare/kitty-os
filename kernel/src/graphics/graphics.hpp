#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <attributes.hpp>
#include <stdint.h>
#include <limine.h>
#include <error_codes.hpp>

#ifndef uint
    typedef unsigned int uint;
#endif

#ifndef uchar
    typedef unsigned char uchar;
#endif

struct TrueColor
{
    uint8_t red;   // Red component (8 bits)
    uint8_t green; // Green component (8 bits)
    uint8_t blue;  // Blue component (8 bits)
} PACKED;

struct TrueColorAlpha
{
    uint8_t red;     // Red component (8 bits)
    uint8_t green;   // Green component (8 bits)
    uint8_t blue;    // Blue component (8 bits)
    uint8_t alpha;   // Alpha (transparency) component (8 bits)
} PACKED;

struct HighColor
{
    uint16_t red   : 5;  // Red component (5 bits)
    uint16_t green : 6;  // Green component (6 bits)
    uint16_t blue  : 5;  // Blue component (5 bits)
} PACKED;

struct Color256
{
    uint8_t index;  // Index into a palette of 256 colors
} PACKED;

struct Color16
{
    uint8_t index  : 4;  // Index into a palette of 16 colors
} PACKED;

struct Color4
{
    uint8_t index  : 2;  // Index into a palette of 4 colors
} PACKED;

// Icon Directory Header
struct ICONDIR {
    uint16_t reserved;   // Reserved (must be 0)
    uint16_t type;       // Resource type (1 for icons)
    uint16_t count;      // Number of icon images in the file
} PACKED;

// Icon Directory Entry
struct ICONDIRENTRY {
    uint8_t width;       // Image width in pixels
    uint8_t height;      // Image height in pixels
    uint8_t colorCount;  // Number of colors (0 if more than 256 colors)
    uint8_t reserved;    // Reserved (must be 0)
    uint16_t planes;     // Color planes (must be 0 or 1)
    uint16_t bitCount;   // Bits per pixel
    uint32_t size;       // Image size in bytes
    uint32_t offset;     // Offset of the image data in the file
} PACKED;

namespace Graphics
{
    extern uint64_t LastError;
    extern limine_framebuffer** limine_framebuffers;
    extern uint framebuffer_count;

    namespace QuickDraw
    {
        ERROR_CODE QDInit();
        void DrawPixel(uint screen_id, uint x, uint y, uchar r, uchar g, uchar b);
        void DrawRectangle(uint screen_id, uint x, uint y, uint w, uint h, uchar r, uchar g, uchar b);
        void DrawFilledRectangle(uint screen_id, uint x, uint y, uint w, uint h, uchar r, uchar g, uchar b);
        void DrawLine(uint screen_id, uint x1, uint y1, uint x2, uint y2, uchar r, uchar g, uchar b);
        void put_bitmap_image_data(uint screen_id, char* bitmap_data, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    }
}

#endif