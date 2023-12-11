#pragma once

#include <stdarg.h>

#define COM1_PORT 0x3F8

void initialize_com1();
void write_to_com1(char character);
void write_string_to_com1(const char* string);
void serial_printf(const char* format, ...);
void print16_hex(unsigned short num);