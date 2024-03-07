#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <stdint.h>

enum SerialPorts
{
    SERIAL_PORT_COM1 = 0x3F8,
    SERIAL_PORT_COM2 = 0x2F8,
    SERIAL_PORT_COM3 = 0x3E8,
    SERIAL_PORT_COM4 = 0x2E8,
    SERIAL_PORT_COM5 = 0x5F8,
    SERIAL_PORT_COM6 = 0x4F8,
    SERIAL_PORT_COM7 = 0x5E8,
    SERIAL_PORT_COM8 = 0x4E8
};

enum SerialDataBits
{
    SERIAL_DATA_BITS_LENGTH_5 = 0b00,
    SERIAL_DATA_BITS_LENGTH_6 = 0b01,
    SERIAL_DATA_BITS_LENGTH_7 = 0b10,
    SERIAL_DATA_BITS_LENGTH_8 = 0b11
};

enum SerialStopBits
{
    SERIAL_STOP_BIT_ONE = 0b100,    
    SERIAL_STOP_BIT_DEPENDING = 0b000   // Depends on the character length, 1.5/2
};

enum SerialParity
{
    SERIAL_PARITY_NONE  =   0b000000,
    SERIAL_PARITY_ODD   =   0b001000,
    SERIAL_PARITY_EVEN  =   0b011000,
    SERIAL_PARITY_MARK  =   0b101000,
    SERIAL_PARITY_SPACE =   0B111000    
};

enum ModemControlBits
{
    DATA_TERMINAL_READY = 0b1,
    REQUEST_TO_SEND     = 0b10,
    HW_PIN_1_EN_IRQ     = 0b1000,
    DIAG_LOOPBACK       = 0b10000
};

struct Baudrate2Divisors
{
    uint8_t msb, lsb;
};

void convert_baudrate_to_divs(uint32_t baudrate, Baudrate2Divisors& b2d);

class SerialPort
{
public:
    SerialPort(enum SerialPorts sPort, uint32_t baud_rate, enum SerialDataBits sdb); // Constructor definition
    ~SerialPort();

    void DisableInterrupts();
    void EnableInterrupts();
    void DisableDLAB();
    void EnableDLAB();
private:
    uint16_t m_Port;
};

#endif
