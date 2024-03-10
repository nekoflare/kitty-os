#include <mt/mt.hpp>
#include <libc/mutex.hpp>
#include <hal/processor/gdt.hpp>
#include <mm/mm.hpp>
#include <hal/processor/smp/smp.hpp>
#include <assert.hpp>
#include <libc/stdio.hpp>
#include <libc/string.hpp>

static Spinlock per_core_init_spinlock;

namespace Multitasking
{
    void PerCoreInitialize(uint64_t core_id)
    {
        // Aquire spin lock
        per_core_init_spinlock.lock();

        // Ask SMP how much cores do we have.
        uint64_t core_count = SMP::get_core_count();

        // Sanity check #1
        assert(sizeof(tss_entry_t) != 0x68, "The size of tss_entry_t is invalid.");

        // Sanity check #2
        assert(core_count == 0, "Invalid amount of cores on the system.\n");

        // Allocate memory for TSS
        void* phys_addr = Memory::alloc_page();
        uint64_t virt_addr = Memory::get_virtual_address((uint64_t)phys_addr);

        // Get the final tss.
        tss_entry_t* this_tss = (tss_entry_t*) virt_addr;

        memset((void*)this_tss, sizeof(tss_entry_t), 0);
    
        // I guess I can set up new stack
        void* syscall_stack_phys = Memory::alloc_page();
        uint64_t syscall_stack_virt = Memory::get_virtual_address((uint64_t)syscall_stack_phys);

        syscall_stack_virt += (PAGE_SIZE - 1); // To the bottom.

        std::printf("RSP0 syscall stack: %llx\n", syscall_stack_virt);
        uint32_t rsp0_low = syscall_stack_virt & 0xffffffff;
        uint32_t rsp0_high = (syscall_stack_virt & 0xffffffff00000000) >> 32;
        std::printf("Low: %llx High: %llx\n", rsp0_low, rsp0_high);

        // Fill it
        // Load RSP0
        this_tss->iopb = sizeof(tss_entry_t);
        this_tss->rsp0_low = rsp0_low;
        this_tss->rsp0_high = rsp0_high;

        system_segment_desc* sys_segment_desc = (system_segment_desc*) (&gdt[6]);

        // base = this_tss
        // flags = 0b0100
        // access byte = 0x89
        // limit = sizeof(tss)
        sys_segment_desc->limit0 = sizeof(tss_entry_t) & 0xFFFF;
        sys_segment_desc->base0 = virt_addr & 0xFFFF;
        sys_segment_desc->base1 = (virt_addr >> 16) & 0xFF;
        sys_segment_desc->access_byte = 0x89;  // Present, Ring 0, Type TSS (available)
        sys_segment_desc->limit1 = (sizeof(tss_entry_t) >> 16) & 0xF;
        sys_segment_desc->flags = 0b1001;  // Granularity = 1, 32-bit TSS
        sys_segment_desc->base2 = (virt_addr >> 24) & 0xFF;
        sys_segment_desc->base3 = (virt_addr >> 32) & 0xFFFFFFFF;
        sys_segment_desc->reserved = 0;

        uint64_t address = sys_segment_desc->base0 | (sys_segment_desc->base1 << 16) | (sys_segment_desc->base2 << 24) | ((uint64_t)sys_segment_desc->base3 << 32);
        std::printf("Moments before disaster: %llx\n", address);
        std::printf("Virt address itself: %llx", virt_addr);

        // Load it
        x86_load_tss();

        // Release spin lock
        per_core_init_spinlock.unlock();
    }
}