//
// Created by Piotr on 14.03.2024.
//

#ifndef KITTY_OS_CPP_SERIAL_H
#define KITTY_OS_CPP_SERIAL_H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

class Serial {
public:
    enum com_ports : uint64_t
    {
        COM1 = 0x3F8,
        COM2 = 0x2F8,
        COM3 = 0x3E8,
        COM4 = 0x2E8,
        COM5 = 0x5F8,
        COM6 = 0x4F8,
        COM7 = 0x5E9,
        COM8 = 0x4E8
    };

// Assume that the clock speed of the UART controller is 115200.
    enum common_divisors : uint64_t
    {
        DIV_115200 = 1,
        DIV_57600 = 2,
        DIV_38400 = 3,
        DIV_28800 = 4,
        DIV_23040 = 5,
        DIV_19200 = 6,
        DIV_14400 = 8,
        DIV_12800 = 9,
        DIV_11520 = 10,
        DIV_9600 = 12,
        DIV_7680 = 15
    };

    #define CHAR_LEN_5 0b00
    #define CHAR_LEN_6 0b01
    #define CHAR_LEN_7 0b10
    #define CHAR_LEN_8 0b11
    #define STOP_BIT_1 0b000    // It's 0
    #define STOP_BIT_OTHER 0b100 // 1.5 / 2 (depending on the character length).
    #define PARITY_NONE 0b0000000
    #define PARITY_ODD 0b001000
    #define PARITY_EVEN 0b011000
    #define PARITY_MARK 0b101000
    #define PARITY_SPACE 0b111000

    #define MODEM_CONTROL_REGISTER_MASK 0b00011111      // Just to mask out the unused bits, we don't want UB when writing them.

    #define UART_INTERNAL_CLOCK 115200
    #define UART_DATA_REGISTER_OFFSET 0
    #define UART_INT_ENABLE_OFFSET 1
    #define UART_DIVISOR_LSB_OFFSET 0   // Note: The setting of DLAB must be set to 1, same for MSB.
    #define UART_DIVISOR_MSB_OFFSET 1
    #define UART_INT_IDENTIFY_FIFO_CTRL_OFFSET 2
    #define UART_LINE_CONTROL_REGISTER_OFFSET 3 // Note: The last bit of this register is the DLAB setting.
    #define UART_MODEM_CONTROL_REGISTER_OFFSET 4
    #define UART_LINE_STATUS_REGISTER_OFFSET 5
    #define UART_MODEM_STATUS_REGISTER_OFFSET 6
    #define UART_SCRATCH_REGISTER_OFFSET 7


    Serial(enum com_ports com_port, enum common_divisors divisor, bool enable_interrupts);

    void EnableDLAB();
    void DisableDLAB();

    void Write(const char chr);
    void Write(const char* msg, size_t len);
    void Print(const char* fmt, ...);

private:
        enum com_ports g_ComPort;           // Current COM port.

        uint64_t g_Speed;                   // Speed of transmission. (bits per second)
        enum common_divisors g_Divisor;     // Current divisor from the list of divisors.
        bool g_IsDLABSet;
        bool g_InterruptsSet;
        uint8_t g_DataFormat;
};


#endif //KITTY_OS_CPP_SERIAL_H
