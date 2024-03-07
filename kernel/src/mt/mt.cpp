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
        void* phys_addr = Memory::AllocPage();
        uint64_t virt_addr = Memory::GetVirtualAddress((uint64_t)phys_addr);

        // Get the final tss.
        tss_entry_t* this_tss = (tss_entry_t*) virt_addr;
    
        // I guess I can set up new stack
        void* syscall_stack_phys = Memory::AllocPage();
        uint64_t syscall_stack_virt = Memory::GetVirtualAddress((uint64_t)syscall_stack_phys);

        // Fill it
        // Load RSP0
        this_tss->io_map_base_address = sizeof(tss_entry_t);
        this_tss->rsp0 = syscall_stack_virt;

        // Set it
        gdt[6].base0 = ((uint64_t)this_tss & 0xffffffff00000000) >> 32;
        gdt[6].base1 = ((uint64_t)this_tss & 0x00000000ffff0000) >> 16;
        gdt[6].base2 = ((uint64_t)this_tss & 0x000000000000ffff);
        gdt[6].limit0 = 0;
        gdt[6].limit1 = sizeof(tss_entry_t);
        gdt[6].access_byte = 0x89;
        gdt[6].flags = 0x40;

        // Load it 
        unsigned short ax = 0x28;
    
        __asm__ __volatile__(
            "ltr %0\n"
            :
            : "a"(ax)
        );

        // Release spin lock
        per_core_init_spinlock.unlock();
    }
}