#include "serial.h"
#include "io.h"
#include <stdbool.h>
#include "string.h"

bool is_serial_initialized = false;

void initialize_com1()
{
    // Disable interrupts on COM1
    outb(COM1_PORT + 1, 0x00);

    // Set the baud rate (for example, 9600 bps)
    uint16_t divisor = 115200 / 9600;
    outb(COM1_PORT + 3, 0x80); // Enable DLAB (Divisor Latch Access Bit)
    outb(COM1_PORT, divisor & 0xFF); // Low byte of divisor
    outb(COM1_PORT + 1, (divisor >> 8) & 0xFF); // High byte of divisor

    // Set data format: 8 bits, no parity, one stop bit
    outb(COM1_PORT + 3, 0x03);

    // Enable FIFO (First-In-First-Out) and clear them
    outb(COM1_PORT + 2, 0xC7);

    // Enable the serial port (out of loopback mode)
    outb(COM1_PORT + 4, 0x0B);
}

void serial_putc(char character)
{
    while (!(inb(COM1_PORT + 5) & 0x20));
    outb(COM1_PORT, character);
}

void serial_puts(const char* string)
{
    int idx = 0;
    while (string[idx] != '\0')
    {
        serial_putc(string[idx]);
        idx++;
    }
}

// Function to print an integer to COM1
void serial_print_int(int num) {
    char buffer[16]; // Assuming a maximum of 16 digits
    int i = 0;

    if (num == 0) {
        serial_putc('0');
        return;
    }

    if (num < 0) {
        serial_putc('-');
        num = -num;
    }

    while (num != 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    for (int j = i - 1; j >= 0; j--) {
        serial_putc(buffer[j]);
    }
}

// Function to print a 64-bit integer to COM1
void serial_print_long(long long num) {
    char buffer[24]; // Assuming a maximum of 24 digits for a 64-bit integer
    int i = 0;

    if (num == 0) {
        serial_putc('0');
        return;
    }

    if (num < 0) {
        serial_putc('-');
        num = -num;
    }

    while (num != 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    for (int j = i - 1; j >= 0; j--) {
        serial_putc(buffer[j]);
    }
}

void print16_hex(uint16_t num) {
    char hex_digits[] = "0123456789ABCDEF";
    char hex[5]; // 16-bit number will have maximum 4 hex digits + '\0'
    int i = 4;
    hex[4] = '\0'; // Null terminator
    
    do {
        hex[--i] = hex_digits[num % 16];
        num /= 16;
    } while (num != 0);

    // Print '0x' prefix
    serial_putc('0');
    serial_putc('x');
    
    // Print the hex characters
    while (hex[i] != '\0') {
        serial_putc(hex[i++]);
    }
    serial_putc('\n');
}


// Function to emulate printf (supports %d, %s, %o, %x, %lld)
void serial_printf(const char* format, ...)
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
                serial_print_int(num);
            }
            else if (*format == 's')
            {
                const char* str = va_arg(args, const char*);
                serial_puts(str);
            }
            else if (*format == 'o')
            {
                unsigned int num = va_arg(args, unsigned int);
                // Use the "o" format specifier for octal
                serial_putc('0');
                serial_print_int(num);
            }
            else if (*format == 'x')
            {
                unsigned int num = va_arg(args, unsigned int);

                // Assuming a 32-bit integer, print in hexadecimal format
                for (int i = 28; i >= 0; i -= 4)
                {
                    char hex_digit = "0123456789ABCDEF"[(num >> i) & 0xF];
                    serial_putc(hex_digit);
                }
            }
            else if (*format == 'l' && *(format + 1) == 'l' && *(format + 2) == 'd')
            {
                long long num = va_arg(args, long long);
                // Use the "lld" format specifier for 64-bit integer
                serial_print_long(num);
                format += 2; // Skip 'l' characters
            }
        }
        else
        {
            serial_putc(*format);
        }
        format++;
    }

    va_end(args);
}