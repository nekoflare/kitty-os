#ifndef MT_HPP
#define MT_HPP

#include <stdint.h>
#include <libc/mutex.hpp>
#include <hal/processor/gdt.hpp>
#include <mm/mm.hpp>

struct tss_entry_t
{
    uint32_t reserved1;
	uint32_t rsp0_low;
	uint32_t rsp0_high;
	uint32_t rsp1_low;
	uint32_t rsp1_high;
	uint32_t rsp2_low;
	uint32_t rsp2_high;
	uint32_t reserved2[2];
	uint32_t ist1_low;
	uint32_t ist1_high;
	uint32_t ist2_low;
	uint32_t ist2_high;
	uint32_t ist3_low;
	uint32_t ist3_high;
	uint32_t ist4_low;
	uint32_t ist4_high;
	uint32_t ist5_low;
	uint32_t ist5_high;
	uint32_t ist6_low;
	uint32_t ist6_high;
	uint32_t ist7_low;
	uint32_t ist7_high;
	uint32_t reserved3[2];
	uint16_t reserved4;
	uint16_t iopb;
} __attribute__((packed));

extern "C" void x86_load_tss();

namespace Multitasking
{
    void PerCoreInitialize(uint64_t core_id);
}

#endif