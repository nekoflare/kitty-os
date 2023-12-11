#pragma once

void dmesg_putc(char c);
void dmesg_puts(const char* s);
void dmesg_print_int(int num);
void dmesg_print_long(long long num);
void dmesg_printf(const char* format, ...);
void dmesg_print_dmesg(void);