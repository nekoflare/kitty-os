#ifndef STDIO_HPP
#define STDIO_HPP

#include <flanterm/flanterm.h>
#include <flanterm/backends/fb.h>
namespace std
{
    extern bool use_truncated_llh;
    extern flanterm_context* ft_ctx;    // Flanterm context
    void init_stdio();          // Initiates the stdio.
    void putc(const char c);    // Prints one character
    void puts(const char* s);   // Prints string
    void print(const char* s);  // It's puts but with new line.
    void print_double(double v);
    void print_float(float v);
    void print_int(int v);
    void print_uint(unsigned int v);
    void print_uint_octal(unsigned int v);
    void print_int_hex(int v, bool isUpper);
    void print_long_long_hex(unsigned long long hexValue, bool uppercase);
    void print_pointer(const void* ptr);
    void print_ulong_long_int(unsigned long long value);
    void print_slong_long_int(signed long long value);
    void sleep(double t);

    void printf(const char* s, ...);
};

#endif