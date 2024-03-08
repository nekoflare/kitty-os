#include <mt/mt.hpp>
#include <libc/mutex.hpp>
#include <hal/processor/gdt.hpp>
#include <mm/mm.hpp>
#include <hal/processor/smp/smp.hpp>
#include <assert.hpp>
#include <libc/stdio.hpp>

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
    
        // I guess I can set up new stack
        void* syscall_stack_phys = Memory::alloc_page();
        uint64_t syscall_stack_virt = Memory::get_virtual_address((uint64_t)syscall_stack_phys);

        syscall_stack_virt += (PAGE_SIZE - 1); // To the bottom.

        // Fill it
        // Load RSP0
        this_tss->iopb = sizeof(tss_entry_t);
        this_tss->rsp0_low = (syscall_stack_virt & 0xffffffff00000000) >> 32;
        this_tss->rsp0_high = syscall_stack_virt & 0x00000000ffffffff;

        // Set it
        gdt[6].base0 = ((uint64_t)this_tss & 0xffffffff00000000) >> 32;
        gdt[6].base1 = ((uint64_t)this_tss & 0x00000000ffff0000) >> 16;
        gdt[6].base2 = ((uint64_t)this_tss & 0x000000000000ffff);
        gdt[6].limit0 = sizeof(tss_entry_t);
        gdt[6].limit1 = 0;
        gdt[6].access_byte = 0x89;
        gdt[6].flags = SIZE_FLAG;

        // Load it
        x86_load_tss();

        // Release spin lock
        per_core_init_spinlock.unlock();
    }
}