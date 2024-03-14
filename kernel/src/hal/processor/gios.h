//
// Created by Piotr on 13.03.2024.
//

#ifndef KITTY_OS_CPP_GIOS_H
#define KITTY_OS_CPP_GIOS_H

#include <stdint.h>
#include <stddef.h>

enum io_type : uint8_t
{
    INVALID = 0,
    MMIO = 1,
    PORTS = 2
};

typedef struct _io_base
{
    uint64_t base_address; // can be mmio or port.
} io_base;

typedef struct _io_handle
{
    io_type _IoType;
    io_base _IoBase;
} io_handle;

uint8_t io_read_byte(io_handle _IoHandle, uint64_t offset);
uint16_t io_read_word(io_handle _IoHandle, uint64_t offset);
uint32_t io_read_dword(io_handle _IoHandle, uint64_t offset);
uint64_t io_read_qword(io_handle _IoHandle, uint64_t offset);
void io_read_buffer(io_handle _IoHandle, uint64_t offset, size_t to_read, char* buffer, size_t buffer_length);

#endif //KITTY_OS_CPP_GIOS_H
