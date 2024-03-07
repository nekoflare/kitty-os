#ifndef MT_HPP
#define MT_HPP

#include <stdint.h>
#include <libc/mutex.hpp>
#include <hal/processor/gdt.hpp>
#include <mm/mm.hpp>

struct tss_entry_t
{
    uint16_t io_map_base_address : 16;
    uint16_t reserved0 : 16;
    uint32_t reserved1, reserved2;
    uint32_t ist7_h, ist7_l;
    uint32_t ist6_h, ist6_l;
    uint32_t ist5_h, ist5_l;
    uint32_t ist4_h, ist4_l;
    uint32_t ist3_h, ist3_l;
    uint32_t ist2_h, ist2_l;
    uint32_t ist1_h, ist1_l;
    uint32_t reserved3, reserved4;
    uint64_t rsp2, rsp1, rsp0;
    uint32_t reserved5;
} __attribute__((packed));

namespace Multitasking
{
    void PerCoreInitialize(uint64_t core_id);
}

#endif