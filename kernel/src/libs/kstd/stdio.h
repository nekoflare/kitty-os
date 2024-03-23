//
// Created by Piotr on 23.03.2024.
//

#ifndef KITTY_OS_CPP_STDIO_H
#define KITTY_OS_CPP_STDIO_H

#include <libs/quickdraw/qdraw.h>
#include <stdarg.h>

#define NRILL_COLORS "\\[b405ee3\\[ff7999e"
#define CUTE_COLORS "\\[fa742f5\\[b251145"

namespace ktl
{
    void init_stdio();

    void putc(char c, Graphics::color_t bg, Graphics::color_t fg);
    void puts(const char* s, Graphics::color_t bg, Graphics::color_t fg);
    void print_unsigned_int(std::uint32_t v, Graphics::color_t bg, Graphics::color_t fg);
    void print_signed_int(std::int32_t v, Graphics::color_t bg, Graphics::color_t fg);
    void print_unsigned_hexadecimal_integer(std::uint32_t v, Graphics::color_t bg, Graphics::color_t fg, bool upper);
    void print_float(float fv, Graphics::color_t bg, Graphics::color_t fg, bool upper);
    void print_scientific_double(double dfv, Graphics::color_t bg, Graphics::color_t fg, bool upper);
    void print_double(double dfv, Graphics::color_t bg, Graphics::color_t fg, bool upper);
    void print_hexadecimal_long_long(std::uint64_t v, Graphics::color_t bg, Graphics::color_t fg, bool upper);
    void print_unsigned_hexadecimal_long_long(std::uint64_t v, Graphics::color_t bg, Graphics::color_t fg, bool upper);
    void printf(const char* fmt, ...);
}

#endif //KITTY_OS_CPP_STDIO_H
