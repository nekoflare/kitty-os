#ifndef GDT_HPP
#define GDT_HPP

#include <stdint.h>

enum AccessByte
{
    PRESENT = 0b10000000,
    RING0   = 0b00000000,
    RING1   = 0b00100000,
    RING2   = 0b01000000,
    RING3   = 0b01100000,
    DESC_TYPE_TSS   = 0b00000000,
    DESC_TYPE_REG   = 0b00010000,
    DESC_TYPE_CODE  = 0b00001000,
    DESC_TYPE_DATA  = 0b00000000,
    DIRECTION_DOWN  = 0b00000100,
    DIRECTION_UP    = 0b00000000,
    READABLE_BIT    = 0b00000010,
    WRITABLE_BIT    = 0b00000010,
    ACCESSED_BIT    = 0b00000001
};

enum Flags
{
    GRAN_4KIB = 0b1000,
    SIZE_FLAG = 0b0100,
    LONG_MODE = 0b0010
};

typedef struct
{
    uint16_t size;
    uint64_t base_address;
} __attribute__((packed)) gdtr_64; 

typedef struct 
{
    uint16_t limit0;
    uint16_t base0;
    uint8_t base1;
    uint8_t access_byte;
    uint8_t limit1 : 4;
    uint8_t flags : 4;
    uint8_t base2;
} __attribute__((packed)) gdt_entry;

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


extern gdt_entry gdt[];

void gdt_init();

#endif

// Thanks to Qwinci for helping a poor soul out there. I love u