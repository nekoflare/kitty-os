//
// Created by Piotr on 14.03.2024.
//

#include <hal/processor/io.hpp>
#include "Serial.h"

using Processor::inb, Processor::outb;

Serial::Serial(enum Serial::com_ports com_port, enum Serial::common_divisors divisor, bool enable_interrupts) {
    g_ComPort = com_port;
    g_Divisor = divisor;
    g_InterruptsSet = enable_interrupts;

    // Begin initializing the COM port.
    // Disable DLAB.
    DisableDLAB();

    // Send low byte of divisor.
    outb(g_ComPort + UART_DIVISOR_LSB_OFFSET, g_Divisor & 0xFF);
    // Send high byte of divisor.
    outb(g_ComPort + UART_DIVISOR_MSB_OFFSET, (g_Divisor >> 8) & 0xFF);

    // Set data format.
    g_DataFormat = CHAR_LEN_8 | STOP_BIT_1 | PARITY_NONE;
    outb(g_ComPort + UART_LINE_CONTROL_REGISTER_OFFSET, g_DataFormat);

    // Enable FIFO.
    // INFO: I don't fucking know what it does.
    outb(g_ComPort + UART_INT_IDENTIFY_FIFO_CTRL_OFFSET, 0xc7);

    // Disable the DLAB.
    DisableDLAB();

    // Enable the serial port (out of loopback mode)
    outb(g_ComPort + UART_MODEM_CONTROL_REGISTER_OFFSET, 0x0B);
}

void Serial::DisableDLAB() {
    uint8_t line_control_register = inb(g_ComPort + UART_LINE_CONTROL_REGISTER_OFFSET); // Read original LCR
    line_control_register &= ~(0b10000000); // Disable DLAB.
    outb(g_ComPort + UART_LINE_CONTROL_REGISTER_OFFSET, line_control_register); // Write back
    g_IsDLABSet = false;
}

void Serial::EnableDLAB() {
    uint8_t line_control_register = inb(g_ComPort + UART_LINE_CONTROL_REGISTER_OFFSET); // Read original LCR
    line_control_register |= 0b10000000; // Enable DLAB.
    outb(g_ComPort + UART_LINE_CONTROL_REGISTER_OFFSET, line_control_register); // Write back
    g_IsDLABSet = true;
}

void Serial::Write(const char chr)
{
    while (!(inb(g_ComPort + UART_LINE_STATUS_REGISTER_OFFSET) & 0x20));
    if (g_IsDLABSet)
    {
        DisableDLAB();
    }

    outb(g_ComPort + UART_DATA_REGISTER_OFFSET, chr);
}

void Serial::Write(const char *msg, size_t len)
{
    for (size_t i = 0; len > i; i++)
    {
        Write(*(msg + i));
    }
}

void Serial::Print(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    int integer;
    long hex;
    void* pointer;
    float number;
    const char* str;

    while (*fmt)
    {
        if (*fmt != '%')
        {
            Write(*fmt);
            ++fmt;
        } else {
            ++fmt;
            switch (*fmt)
            {
                case 'd':
                    integer = va_arg(args, int);
                    Write("Number\n", 6);
                    break;
                case 'x':
                    hex = va_arg(args, long);
                    Write("32-bit Hex\n", 11);
                    break;
                case 'p':
                    pointer = va_arg(args, void*);
                    Write("Pointer\n", 8);
                    break;
                case 'f':
                    number = (float) va_arg(args, unsigned long);
                    Write("32-bit Float\n", 13);
                    break;
                case 's':
                    str = va_arg(args, const char*);
                    Write("String\n", 7);
                    break;
                default:
                    Write("Unknown\n", 8);
            }
        }
    }

    va_end(args);
}