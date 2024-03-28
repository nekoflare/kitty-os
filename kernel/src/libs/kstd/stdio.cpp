//
// Created by Piotr on 23.03.2024.
//

#include <cstdint>
#include <libs/flanterm/backends/fb.h>
#include <libs/flanterm/flanterm.h>
#include <kernel/structures.h>
#include <libs/kstd/string.h>
#include <libs/math/math.h>
#include <libs/kstd/stdio.h>


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

        char c;
        const char* s;
        float f32;
        double f64;
        std::uint8_t u8;
        std::uint16_t u16;
        std::uint32_t u32;
        std::uint64_t u64;
        void* p;

        while (*format)
        {
            if (*format == '%')
            {
                ++format;
                switch (*format)
                {
                    case 'i': // 32-bit signed integer
                    case 'd': // 32-bit signed integer
                        u32 = va_arg(args, signed int);
                        ktl::print_signed_int(u32, bg, fg);
                        ++format;
                        break;
                    case 'u':
                        u32 = va_arg(args, unsigned int);
                        ktl::print_unsigned_int(u32, bg, fg);
                        ++format;
                        break;
                    case 'x':
                        u32 = va_arg(args, int);
                        ++format;
                        ktl::print_unsigned_hexadecimal_integer(u32, bg, fg, false);
                        break;
                    case 'X':
                        u32 = va_arg(args, int);
                        ++format;
                        ktl::print_unsigned_hexadecimal_integer(u32, bg, fg, true);
                        break;
                    case 'f':
                        f32 = static_cast<float>(va_arg(args, double));
                        ++format;
                        ktl::print_float(f32, bg, fg, false);
                        break;
                    case 'F':
                        f32 = static_cast<float>(va_arg(args, double));
                        ++format;
                        ktl::print_float(f32, bg, fg, true);
                        break;
                    case 'e':
                        f64 = va_arg(args, double);
                        ++format;
                        ktl::print_scientific_double(f64, bg, fg, false);
                        break;
                    case 'E':
                        f64 = va_arg(args, double);
                        ++format;
                        ktl::print_scientific_double(f64, bg, fg, true);
                        break;
                    case 'c':
                        c = static_cast<char>(va_arg(args, int));
                        ++format;
                        ktl::putc(c, bg, fg);
                        break;
                    case 's':
                        s = va_arg(args, const char*);
                        ++format;
                        ktl::puts(s, bg, fg);
                        break;
                    case 'p':
                        p = va_arg(args, void*);
                        ++format;
                        ktl::print_hexadecimal_long_long(reinterpret_cast<std::uint64_t>(p), bg, fg, false);
                        break;
                    case 'P':
                        p = va_arg(args, void*);
                        ++format;
                        ktl::print_hexadecimal_long_long(reinterpret_cast<std::uint64_t>(p), bg, fg, true);
                        break;
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
                    case '\\':
                        ktl::putc(*format, bg, fg);
                        ++format;
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

    void print_unsigned_int(std::uint32_t v, Graphics::color_t bg, Graphics::color_t fg) {
        // Extract digits of the unsigned integer value and print each one
        // from left to right using ktl::putc
        if (v == 0) {
            ktl::putc('0', bg, fg);
            return;
        }

        char buffer[12]; // Assuming max length of uint32_t string representation is 11 characters
        int index = 0;

        // Extract digits in reverse order
        while (v != 0) {
            buffer[index++] = '0' + (v % 10);
            v /= 10;
        }

        // Print digits in forward order
        for (int i = index - 1; i >= 0; --i) {
            ktl::putc(buffer[i], bg, fg);
        }
    }

    void print_unsigned_hexadecimal_integer(std::uint32_t v, Graphics::color_t bg, Graphics::color_t fg, bool upper) {
        // Buffer to store hexadecimal digits
        char buffer[8]; // Enough to store a 32-bit hexadecimal value
        int i = 0;

        // Convert integer to hexadecimal string
        do {
            int digit = v % 16;
            buffer[i++] = digit < 10 ? '0' + digit : (upper ? 'A' : 'a') + digit - 10;
            v /= 16;
        } while (v);

        // Print digits in reverse order
        while (i > 0) {
            ktl::putc(buffer[--i], bg, fg);
        }
    }

    void print_float(float fv, Graphics::color_t bg, Graphics::color_t fg, bool upper) {
        // Check for negative number
        if (fv < 0) {
            ktl::putc('-', bg, fg);
            fv = -fv;
        }

        // Handle infinities and NaN
        if (ktl::isinf(fv) || ktl::isnan(fv)) {
            // Print "inf" or "nan" as appropriate
            const char* special = ktl::isnan(fv) ? "nan" : "inf";
            for (int i = 0; special[i] != '\0'; ++i) {
                ktl::putc(special[i], bg, fg);
            }
            return;
        }

        // Print the integer part
        int integer_part = static_cast<int>(fv);
        print_unsigned_hexadecimal_integer(integer_part, bg, fg, upper);

        // Print the decimal point
        ktl::putc('.', bg, fg);

        // Print the fractional part
        float fractional_part = fv - integer_part;
        const int max_decimal_places = 6; // Maximum precision
        for (int i = 0; i < max_decimal_places; ++i) {
            fractional_part *= 10;
            int digit = static_cast<int>(fractional_part);
            ktl::putc('0' + digit, bg, fg);
            fractional_part -= digit;
        }

        // Optionally, print scientific notation
        if (upper) {
            ktl::putc('E', bg, fg);
        } else {
            ktl::putc('e', bg, fg);
        }

        // Print the exponent
        int exponent = static_cast<int>(ktl::log10(ktl::abs(fv)));
        print_signed_int(exponent, bg, fg);
    }

    void print_double(double dfv, Graphics::color_t bg, Graphics::color_t fg, bool upper) {
        // Check for negative number
        if (dfv < 0) {
            ktl::putc('-', bg, fg);
            dfv = -dfv;
        }

        // Handle infinities and NaN
        if (ktl::isinf(dfv) || ktl::isnan(dfv)) {
            // Print "inf" or "nan" as appropriate
            const char* special = ktl::isnan(dfv) ? "nan" : "inf";
            for (int i = 0; special[i] != '\0'; ++i) {
                ktl::putc(special[i], bg, fg);
            }
            return;
        }

        // Print the integer part
        long long integer_part = static_cast<long long>(dfv);
        print_unsigned_hexadecimal_long_long(integer_part, bg, fg, upper);

        // Print the decimal point
        ktl::putc('.', bg, fg);

        // Print the fractional part
        double fractional_part = dfv - integer_part;
        const int max_decimal_places = 6; // Maximum precision
        for (int i = 0; i < max_decimal_places; ++i) {
            fractional_part *= 10;
            int digit = static_cast<int>(fractional_part);
            ktl::putc('0' + digit, bg, fg);
            fractional_part -= digit;
        }

        // Optionally, print scientific notation
        if (upper) {
            ktl::putc('E', bg, fg);
        } else {
            ktl::putc('e', bg, fg);
        }

        // Print the exponent
        int exponent = static_cast<int>(ktl::log10(ktl::abs(dfv)));
        print_signed_int(exponent, bg, fg);
    }

    void print_scientific_double(double dfv, Graphics::color_t bg, Graphics::color_t fg, bool upper) {
        // Check for negative number
        if (dfv < 0) {
            ktl::putc('-', bg, fg);
            dfv = -dfv;
        }

        // Handle infinities and NaN
        if (ktl::isinf(dfv) || ktl::isnan(dfv)) {
            // Print "inf" or "nan" as appropriate
            const char* special = ktl::isnan(dfv) ? "nan" : "inf";
            for (int i = 0; special[i] != '\0'; ++i) {
                ktl::putc(special[i], bg, fg);
            }
            return;
        }

        // Extract exponent
        int exponent = static_cast<int>(ktl::log10(ktl::abs(dfv)));

        // Extract significand
        double significand = dfv / ktl::pow(10, exponent);

        // Print significand
        if (significand >= 1.0) {
            print_double(significand, bg, fg, upper);
        } else {
            ktl::putc('0', bg, fg);
            ktl::putc('.', bg, fg);
            print_double(significand, bg, fg, upper);
        }

        // Print exponent
        if (upper) {
            ktl::putc('E', bg, fg);
        } else {
            ktl::putc('e', bg, fg);
        }
        print_signed_int(exponent, bg, fg);
    }

    void print_hexadecimal_long_long(std::uint64_t v, Graphics::color_t bg, Graphics::color_t fg, bool upper) {
        // Buffer to store hexadecimal digits
        char buffer[17]; // Enough to store a 64-bit hexadecimal value
        int i = 0;

        // Convert integer to hexadecimal string
        do {
            int digit = v & 0xF; // Extract lowest 4 bits
            buffer[i++] = digit < 10 ? '0' + digit : (upper ? 'A' : 'a') + digit - 10;
            v >>= 4; // Shift to get next 4 bits
        } while (v);

        // Print digits in reverse order
        while (i > 0) {
            ktl::putc(buffer[--i], bg, fg);
        }
    }

    void print_unsigned_hexadecimal_long_long(std::uint64_t v, Graphics::color_t bg, Graphics::color_t fg, bool upper) {
        // Buffer to store hexadecimal digits
        char buffer[17]; // Enough to store a 64-bit hexadecimal value
        int i = 0;

        // Convert integer to hexadecimal string
        do {
            int digit = v & 0xF; // Extract lowest 4 bits
            buffer[i++] = digit < 10 ? '0' + digit : (upper ? 'A' : 'a') + digit - 10;
            v >>= 4; // Shift to get next 4 bits
        } while (v);

        // Print digits in reverse order
        while (i > 0) {
            ktl::putc(buffer[--i], bg, fg);
        }
    }

    void print_signed_int(std::int32_t value, Graphics::color_t bg, Graphics::color_t fg) {
        // Handle negative numbers
        if (value < 0) {
            ktl::putc('-', bg, fg);
            value = -value;
        }

        // Handle zero separately
        if (value == 0) {
            ktl::putc('0', bg, fg);
            return;
        }

        // Buffer to store digits
        char buffer[12]; // Enough to hold INT32_MIN in base 10
        int i = 0;

        // Extract digits in reverse order
        while (value != 0) {
            buffer[i++] = '0' + (value % 10);
            value /= 10;
        }

        // Print digits in correct order
        while (i > 0) {
            ktl::putc(buffer[--i], bg, fg);
        }
    }
}