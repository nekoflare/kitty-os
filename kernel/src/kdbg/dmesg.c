#include <kdbg/dmesg.h>
#include <stdarg.h>
#include <klibc/stdio.h>
#include <io/serial.h>

unsigned long dmesg_length = 8192;
unsigned long dmesg_index = 0;
char dmesg_buffer[8192];

void dmesg_putc(char c)
{
    if (dmesg_index >= dmesg_length)
    {
        dmesg_index = 0; // loop back!
    }

    dmesg_buffer[dmesg_index] = c;
}

void dmesg_puts(const char* s)
{
    unsigned long idx = 0;
    while (s[idx] != 0)
    {
        dmesg_putc(s[idx]);
        idx++;
    }
}

// Function to print an integer to COM1
void dmesg_print_int(int num) {
    char buffer[16]; // Assuming a maximum of 16 digits
    int i = 0;

    if (num == 0) {
        dmesg_putc('0');
        return;
    }

    if (num < 0) {
        dmesg_putc('-');
        num = -num;
    }

    while (num != 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    for (int j = i - 1; j >= 0; j--) {
        dmesg_putc(buffer[j]);
    }
}


void dmesg_print_long(long long num) {
    char buffer[24]; // Assuming a maximum of 24 digits for a 64-bit integer
    int i = 0;

    if (num == 0) {
        dmesg_putc('0');
        return;
    }

    if (num < 0) {
        dmesg_putc('-');
        num = -num;
    }

    while (num != 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    for (int j = i - 1; j >= 0; j--) {
        dmesg_putc(buffer[j]);
    }
}

void dmesg_printf(const char* format, ...)
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
                dmesg_print_int(num);
            }
            else if (*format == 's')
            {
                const char* str = va_arg(args, const char*);
                dmesg_puts(str);
            }
            else if (*format == 'o')
            {
                unsigned int num = va_arg(args, unsigned int);
                // Use the "o" format specifier for octal
                dmesg_putc('0');
                dmesg_print_int(num);
            }
            else if (*format == 'x')
            {
                unsigned int num = va_arg(args, unsigned int);

                // Assuming a 32-bit integer, print in hexadecimal format
                for (int i = 28; i >= 0; i -= 4)
                {
                    char hex_digit = "0123456789ABCDEF"[(num >> i) & 0xF];
                    dmesg_putc(hex_digit);
                }
            }
            else if (*format == 'l' && *(format + 1) == 'l' && *(format + 2) == 'd')
            {
                long long num = va_arg(args, long long);
                // Use the "lld" format specifier for 64-bit integer
                dmesg_print_long(num);
                format += 2; // Skip 'l' characters
            }
        }
        else
        {
            dmesg_putc(*format);
        }
        format++;
    }

    va_end(args);
}

void dmesg_print_dmesg(void)
{
    kputs(dmesg_buffer);
}