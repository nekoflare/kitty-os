//
// Created by Piotr on 13.03.2024.
//

#include <hal/processor/io.hpp>
#include "gios.h"

uint8_t io_read_byte(io_handle _IoHandle, uint64_t offset)
{
    switch (_IoHandle._IoType)
    {
        case MMIO:
            return *(uint8_t*)(_IoHandle._IoBase.base_address + offset);
        case PORTS:
            return Processor::inb(_IoHandle._IoBase.base_address + offset);
        default:
            return 0;
    }

    return 0;
}