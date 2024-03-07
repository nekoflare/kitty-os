#include <drivers/serial/serial.hpp>
#include <hal/processor/io.hpp>

void convert_baudrate_to_divs(uint32_t baudrate, Baudrate2Divisors& b2d) {
    // Internal clock frequency
    uint32_t internalClockFrequency = 115200;

    // Calculate divisor
    uint32_t divisor = internalClockFrequency / baudrate;

    // Check if divisor is within valid range (1 to 65535)
    if (divisor > 65535) {
        b2d.lsb = 0;
        b2d.msb = 0;
        return;
    }

    // Split into MSB and LSB
    b2d.lsb = divisor & 0xFF;  // Least significant byte
    b2d.msb = (divisor >> 8) & 0xFF;  // Most significant byte
}

void SerialPort::DisableInterrupts()
{
    Processor::outb(m_Port + 1, 0x00);    // Disable Interrupts.
}

void SerialPort::EnableInterrupts()
{
    Processor::outb(m_Port + 1, 0x01);    // Enable Interrupts

}

void SerialPort::DisableDLAB()
{
    Processor::outb(m_Port + 3, 0x00);    // Disable DLAB.
}

void SerialPort::EnableDLAB()
{
    Processor::outb(m_Port + 3, 0x80);    // Enable DLAB.
}

SerialPort::SerialPort(enum SerialPorts sPort, uint32_t baud_rate, enum SerialDataBits sdb)
{
    m_Port = sPort;
    this->DisableDLAB();
    this->DisableInterrupts();

    // Calculate divisors.
    Baudrate2Divisors b2d;
    convert_baudrate_to_divs(baud_rate, b2d);
    if (b2d.lsb == 0 && b2d.msb == 0)
    {
        // failed!
        return;
    }

    // Set baudrate.
    this->EnableDLAB();
    Processor::outb(m_Port, b2d.lsb);   // LSB of the divisor
    Processor::outb(m_Port + 1, b2d.msb);   // MSB of the divisor.
    this->DisableDLAB();

    Processor::outb(m_Port + 3, 0x3); // no parity, one stop bit, 8 bits
}