//
// Created by Piotr on 14.04.2024.
//

#ifndef KITTY_OS_CPP_GDT_H
#define KITTY_OS_CPP_GDT_H

#include <cstdint>

enum GDTAccessByte
{
    GDT_PRESENT = 0b10000000,
    GDT_RING0   = 0b00000000,
    GDT_RING1   = 0b00100000,
    GDT_RING2   = 0b01000000,
    GDT_RING3   = 0b01100000,
    GDT_DESC_TYPE_TSS   = 0b00000000,
    GDT_DESC_TYPE_REG   = 0b00010000,
    GDT_DESC_TYPE_CODE  = 0b00001000,
    GDT_DESC_TYPE_DATA  = 0b00000000,
    GDT_DIRECTION_DOWN  = 0b00000100,
    GDT_DIRECTION_UP    = 0b00000000,
    GDT_READABLE_BIT    = 0b00000010,
    GDT_WRITABLE_BIT    = 0b00000010,
    GDT_ACCESSED_BIT    = 0b00000001
};

enum GDTFlags
{
    GDT_GRAN_4KIB = 0b1000,
    GDT_SIZE_FLAG = 0b0100,
    GDT_LONG_MODE = 0b0010
};

typedef struct
{
    std::uint16_t limit0;
    std::uint16_t base0;
    std::uint8_t base1;
    std::uint8_t access_byte;
    std::uint8_t limit1 : 4;
    std::uint8_t flags : 4;
    std::uint8_t base2;
} __attribute__((packed)) GDTEntry;

typedef struct
{
    std::uint16_t limit0;
    std::uint16_t base0;
    std::uint8_t base1;
    std::uint8_t access_byte;
    std::uint8_t limit1 : 4;
    std::uint8_t flags : 4;
    std::uint8_t base2;
    std::uint32_t base3;
    std::uint32_t reserved;
} __attribute__((packed)) GDTSystemSegmentDescriptor;

typedef struct
{
    std::uint16_t size;
    std::uint64_t address;
} __attribute((packed)) GDTRegister64;

#define GDT_ENTRY(base_val, flags_val, limit_val, access_val) \
    {                                                         \
        .limit0 = (limit_val) & 0xFFFF,                       \
        .base0 = (base_val) & 0xFFFF,                         \
        .base1 = ((base_val) >> 16) & 0xFF,                   \
        .access_byte = (access_val),                          \
        .limit1 = ((limit_val) >> 16) & 0x0F,                 \
        .flags = (flags_val),                                 \
        .base2 = ((base_val) >> 24) & 0xFF                    \
    }

extern "C" void KeASMReloadSegments(GDTRegister64* gdtr);
void KeFlushGlobalDescriptorTable();

#endif //KITTY_OS_CPP_GDT_H
