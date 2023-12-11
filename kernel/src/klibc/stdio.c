#include "stdio.h"
#include "psf2.h"
#include "bgraphics/graphics.h"
#include <stdarg.h>

unsigned int x = 0;
unsigned int y = 0;

void clean_stdio()
{
    x = 0;
    y = 0;
}

void kputc(char c)
{  
    if (x > this_framebuffer->width)
    {
        x = 0;
        y += psf2_header->height;
    }

    if (y > this_framebuffer->height)
    {
        y = 0;
        put_rect(0, 0, this_framebuffer->width, this_framebuffer->height, 0, 0, 0);
    }

    if (c == '\n')
    {
        x = 0;
        y += psf2_header->height;

        return;
    }

    draw_character_psf2(x, y, c, 255, 255, 255, 1, 1);
    x += psf2_header->width;
}

void kputs(const char* s)
{
    unsigned int idx = 0;
    while (s[idx] != 0)
    {
        kputc(s[idx]);
        idx++;
    }
}

// Function to print an integer to COM1
void kprint_int(int num) {
    char buffer[16]; // Assuming a maximum of 16 digits
    int i = 0;

    if (num == 0) {
        kputc('0');
        return;
    }

    if (num < 0) {
        kputc('-');
        num = -num;
    }

    while (num != 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    for (int j = i - 1; j >= 0; j--) {
        kputc(buffer[j]);
    }
}

// Function to print a 64-bit integer to COM1
void kprint_long(long long num) {
    char buffer[24]; // Assuming a maximum of 24 digits for a 64-bit integer
    int i = 0;

    if (num == 0) {
        kputc('0');
        return;
    }

    if (num < 0) {
        kputc('-');
        num = -num;
    }

    while (num != 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    for (int j = i - 1; j >= 0; j--) {
        kputc(buffer[j]);
    }
}

void kprintf(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    while (*format)
    {
        if (*format == '%')
        {
            format++;
            if (*format == 'd')
            {
                int num = va_arg(args, int);
                kprint_int(num);
            }
            else if (*format == 's')
            {
                const char* str = va_arg(args, const char*);
                kputs(str);
            }
            else if (*format == 'o')
            {
                unsigned int num = va_arg(args, unsigned int);
                // Use the "o" format specifier for octal
                kputc('0');
                kprint_int(num);
            }
            else if (*format == 'x')
            {
                unsigned int num = va_arg(args, unsigned int);

                // Assuming a 32-bit integer, print in hexadecimal format
                for (int i = 28; i >= 0; i -= 4)
                {
                    char hex_digit = "0123456789ABCDEF"[(num >> i) & 0xF];
                    kputc(hex_digit);
                }
            }
            else if (*format == 'l' && *(format + 1) == 'l' && *(format + 2) == 'd')
            {
                long long num = va_arg(args, long long);
                // Use the "lld" format specifier for 64-bit integer
                kprint_long(num);
                format += 2; // Skip 'l' characters
            }
        }
        else
        {
            kputc(*format);
        }
        format++;
    }

    va_end(args);
}