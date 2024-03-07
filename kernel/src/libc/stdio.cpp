#include <libc/stdio.hpp>
#include <flanterm/flanterm.h>
#include <flanterm/backends/fb.h>
#include <graphics/graphics.hpp>
#include <libc/string.hpp>
#include <math/math.hpp>
#include <hal/processor/access.hpp>
#include <hal/processor/speed.hpp>
#include <stdarg.h>
#include <libc/mutex.hpp>

namespace std
{
    flanterm_context* ft_ctx = nullptr;
}

static Spinlock putcSl;
static Spinlock putsSl;
static Spinlock printSl;
static Spinlock printfSl;

void std::init_stdio()
{
    std::ft_ctx = flanterm_fb_simple_init(
        static_cast<uint32_t*>(Graphics::limine_framebuffers[0]->address), Graphics::limine_framebuffers[0]->width, Graphics::limine_framebuffers[0]->height, Graphics::limine_framebuffers[0]->pitch
    );
}

void std::putc(const char c)
{
    putcSl.lock();
    flanterm_write(std::ft_ctx, &c, 1);
    putcSl.unlock();
}

void std::puts(const char* s)
{
    putsSl.lock();
    flanterm_write(std::ft_ctx, s, strlen(s));
    putsSl.unlock();
}

void std::print(const char* s)
{
    printSl.lock();
    std::puts(s);
    std::putc('\n');
    printSl.unlock();
}

void std::print_double(double v)
{
    // Handle negative numbers
    if (v < 0) {
        std::putc('-');
        v = -v;
    }

    // Extract integer part
    unsigned long long int_part = static_cast<unsigned long long>(v);
    unsigned long long fractional_part = static_cast<unsigned long long>((v - int_part) * 1000000);

    // Print integer part
    if (int_part == 0) {
        std::putc('0');
    } else {
        unsigned int digits = Math::log10(int_part) + 1;
        for (unsigned int i = 0; i < digits; ++i) {
            unsigned long long divisor = Math::pow(10, digits - i - 1);
            std::putc('0' + int_part / divisor);
            int_part %= divisor;
        }
    }

    // Print decimal point
    std::putc('.');

    // Print fractional part
    std::putc('0' + (fractional_part / 100000) % 10);
    std::putc('0' + (fractional_part / 10000) % 10);
    std::putc('0' + (fractional_part / 1000) % 10);
    std::putc('0' + (fractional_part / 100) % 10);
    std::putc('0' + (fractional_part / 10) % 10);
    std::putc('0' + fractional_part % 10);
}

void std::sleep(double t)
{
    double cpu_speed_mhz = Processor::get_cpu_speed_mhz();

    // Calculate the number of CPU cycles needed to sleep for 't' seconds
    uint64_t cycles_to_sleep = static_cast<uint64_t>(t * cpu_speed_mhz * 1000000.0 * 256);

    // Measure the start time using rdtsc
    uint64_t start = Processor::rdtsc();

    // Busy-wait until the required number of cycles have elapsed
    while (Processor::rdtsc() - start < cycles_to_sleep);
}

void std::print_int(int v) {
        // Handle negative numbers
        if (v < 0) {
            std::putc('-');
            v = -v; // Make it positive for further processing
        }
        
        // Temporary array to store digits
        char buffer[20]; // Maximum number of digits in a 32-bit integer
        
        // Index to store digits in the buffer
        int idx = 0;
        
        // Special case for 0
        if (v == 0) {
            std::putc('0');
            return;
        }
        
        // Extract digits one by one and store them in the buffer
        while (v != 0) {
            int digit = v % 10; // Extract the last digit
            buffer[idx++] = '0' + digit; // Convert digit to character and store it in the buffer
            v /= 10; // Move to the next digit
        }
        
        // Print digits in reverse order (stored in the buffer)
        for (int i = idx - 1; i >= 0; --i) {
            std::putc(buffer[i]);
        }
}

void std::print_uint(unsigned int v)
{
    // Edge case: if the value is 0, print it immediately
    if (v == 0) {
        std::putc('0');
        return;
    }

    // Store digits in reverse order in a temporary buffer
    char buffer[20]; // Assuming 32-bit unsigned int, 20 digits is enough
    int index = 0;
    while (v != 0) {
        buffer[index++] = '0' + (v % 10);
        v /= 10;
    }

    // Print digits in correct order
    for (int i = index - 1; i >= 0; i--) {
        std::putc(buffer[i]);
    }
}

void std::print_uint_octal(unsigned int v) {
    // Edge case: if the value is 0, print it immediately
    if (v == 0) {
        std::putc('0');
        return;
    }

    // Store digits in reverse order in a temporary buffer
    char buffer[20]; // Assuming 32-bit unsigned int, 20 digits is enough
    int index = 0;
    while (v != 0) {
        buffer[index++] = '0' + (v % 8); // Convert to octal
        v /= 8;
    }

    // Print digits in correct order
    for (int i = index - 1; i >= 0; i--) {
        std::putc(buffer[i]);
    }
}

void std::print_int_hex(int v, bool isUpper) {
    // Edge case: if the value is 0, print it immediately
    if (v == 0) {
        std::putc('0');
        return;
    }

    // Store digits in reverse order in a temporary buffer
    char buffer[47]; // Assuming 32-bit integer, 20 digits is enough
    int index = 0;
    while (v != 0) {
        int digit = v % 16;
        if (digit < 10)
            buffer[index++] = '0' + digit; // Convert to ASCII character
        else
            buffer[index++] = (isUpper ? 'A' : 'a') + digit - 10; // Convert to ASCII character (A-F or a-f)
        v /= 16;
    }

    // Print digits in correct order
    for (int i = index - 1; i >= 0; i--) {
        std::putc(buffer[i]);
    }
}

void std::print_float(float v) {
    // Handle negative values
    if (v < 0) {
        std::putc('-');
        v = -v;
    }

    // Extract integer part
    int integerPart = static_cast<int>(v);
    float fractionalPart = v - integerPart;

    // Print integer part
    int digitCount = 0;
    int tempIntegerPart = integerPart;
    do {
        ++digitCount;
        tempIntegerPart /= 10;
    } while (tempIntegerPart > 0);

    for (int i = digitCount - 1; i >= 0; --i) {
        int digit = (integerPart / static_cast<int>(Math::pow(10, i))) % 10;
        std::putc('0' + digit);
    }

    // Print decimal point
    std::putc('.');

    // Print fractional part
    const int precision = 6; // Adjust precision as needed
    for (int i = 0; i < precision; ++i) {
        fractionalPart *= 10;
        int digit = static_cast<int>(fractionalPart) % 10;
        std::putc('0' + digit);
    }
}

bool std::use_truncated_llh = true;

void std::print_long_long_hex(unsigned long long hexValue, bool uppercase) {
    if (use_truncated_llh)
    {
        const char* hexDigits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
        bool leadingZero = true; // Flag to track leading zeros

        for (int i = 60; i >= 0; i -= 4) {
            char ch = hexDigits[(hexValue >> i) & 0xF];
            if (ch != '0' || !leadingZero) {
                std::putc(ch); // Print the character
                leadingZero = false; // Update flag
            }
        }

        if (leadingZero) {
            // If all digits are zero, print at least one zero
            std::putc('0');
        }

        return;
    }
    
    const char* hexDigits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";

    for (int i = 60; i >= 0; i -= 4) {
        char ch = hexDigits[(hexValue >> i) & 0xF];
        std::putc(ch); // Print the character
    }
}

void std::print_pointer(const void* ptr) {
    const uintptr_t address = reinterpret_cast<uintptr_t>(ptr);
    char buffer[18]; // Long enough to hold a 64-bit pointer address in hexadecimal form
    buffer[0] = '0';
    buffer[1] = 'x';
    for (int i = 2; i < 18; ++i) {
        const int nibble = (address >> (4 * (15 - i))) & 0xF;
        buffer[i] = (nibble < 10) ? ('0' + nibble) : ('a' + nibble - 10);
    }
    buffer[17] = '\0';
    std::print(buffer);
}

void std::print_ulong_long_int(unsigned long long value)
{
    // Handle the case where the value is 0 separately
    if (value == 0) {
        std::putc('0');
        return;
    }

    char buffer[21]; // Enough space to store a 20-digit number + null terminator
    int index = 0;

    // Extract digits from least significant to most significant
    while (value > 0) {
        buffer[index++] = '0' + (value % 10);
        value /= 10;
    }

    // Print digits in reverse order
    while (index > 0) {
        std::putc(buffer[--index]);
    }
}

void std::print_slong_long_int(signed long long value)
{
    // Handle the sign separately
    if (value < 0) {
        std::putc('-');
        value = -value; // Make value positive
    }
    std::print_ulong_long_int(static_cast<unsigned long long>(value));
}

void std::printf(const char* format, ...) {
    printfSl.lock();

    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            ++format; // Move past the '%'
            switch (*format) {
                case 'c':
                    putc(va_arg(args, int)); // Characters are promoted to int in variadic functions
                    break;
                case 's':
                    puts(va_arg(args, const char*));
                    break;
                case 'd':
                    print_int(va_arg(args, int));
                    break;
                case 'u':
                    print_uint(va_arg(args, unsigned int));
                    break;
                case 'x':
                    print_int_hex(va_arg(args, unsigned int), false);
                    break;
                case 'X':
                    print_int_hex(va_arg(args, unsigned int), true);
                    break;
                case 'o':
                    print_uint_octal(va_arg(args, unsigned int));
                    break;
                case 'f':
                    print_float(va_arg(args, double));
                    break;
                case 'p':
                    print_pointer(va_arg(args, const void*));
                    break;
                case 'l':
                    ++format; // Move past 'l'
                    if (*format == 'l') {
                        ++format; // Move past second 'l' for long long
                        if (*format == 'x' || *format == 'X') {
                            print_long_long_hex(va_arg(args, uint64_t), *format == 'X');
                            break;
                        } else if (*format == 'd')
                        {
                            std::print_ulong_long_int(va_arg(args, uint64_t));
                            break;
                        } else if (*format == 'u')
                        {
                            std::print_slong_long_int(va_arg(args, uint64_t));
                            break;
                        }

                    }
                    // If format doesn't match expected, just print 'l'
                    putc('l');
                    break;
                default:
                    putc(*format); // Print the character if unknown specifier
                    break;
            }
        } else {
            putc(*format); // Print normal characters
        }
        ++format;
    }

    va_end(args);
    printfSl.unlock();
}
