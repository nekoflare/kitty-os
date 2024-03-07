#ifndef PROCESSOR_IO_HPP
#define PROCESSOR_IO_HPP

#include <cstdint>

namespace Processor
{
        inline std::uint8_t inb(std::uint16_t port)
        {
            std::uint8_t value;
            asm volatile ("inb %1, %0" : "=a"(value) : "d"(port));
            return value;
        }

        inline std::uint16_t inw(std::uint16_t port)
        {
            std::uint16_t value;
            asm volatile ("inw %1, %0" : "=a"(value) : "d"(port));
            return value;
        }

        inline void outb(std::uint16_t port, std::uint8_t value)
        {
            asm volatile ("outb %0, %1" : : "a"(value), "d"(port));
        }

        inline void outw(std::uint16_t port, std::uint16_t value)
        {
            asm volatile ("outw %0, %1" : : "a"(value), "d"(port));
        }

        inline std::uint32_t inl(std::uint16_t port)
        {
            std::uint32_t value;
            asm volatile ("inl %1, %0" : "=a"(value) : "d"(port));
            return value;
        }

        inline void outl(std::uint16_t port, std::uint32_t value)
        {
            asm volatile ("outl %0, %1" : : "a"(value), "d"(port));
        }

        inline void insb(std::uint16_t port, std::uint8_t* buffer, std::size_t count)
        {
            asm volatile ("cld; rep insb"
                          : "+D"(buffer), "+c"(count)
                          : "d"(port)
                          : "memory");
        }

        inline void insw(std::uint16_t port, std::uint16_t* buffer, std::size_t count)
        {
            asm volatile ("cld; rep insw"
                          : "+D"(buffer), "+c"(count)
                          : "d"(port)
                          : "memory");
        }

        inline void insl(std::uint16_t port, std::uint32_t* buffer, std::size_t count)
        {
            asm volatile ("cld; rep insl"
                          : "+D"(buffer), "+c"(count)
                          : "d"(port)
                          : "memory");
        }

        inline void outsb(std::uint16_t port, const std::uint8_t* buffer, std::size_t count)
        {
            asm volatile ("cld; rep outsb"
                          : "+S"(buffer), "+c"(count)
                          : "d"(port));
        }

        inline void outsw(std::uint16_t port, const std::uint16_t* buffer, std::size_t count)
        {
            asm volatile ("cld; rep outsw"
                          : "+S"(buffer), "+c"(count)
                          : "d"(port));
        }

        inline void outsl(std::uint16_t port, const std::uint32_t* buffer, std::size_t count)
        {
            asm volatile ("cld; rep outsl"
                          : "+S"(buffer), "+c"(count)
                          : "d"(port));
        }

        inline void iowait()
        {
            outb(0x80, 0);
        }
}

#endif
