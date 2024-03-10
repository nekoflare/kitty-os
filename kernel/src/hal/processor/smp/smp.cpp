#include <hal/processor/smp/smp.hpp>
#include <libc/stdio.hpp>
#include <libc/mutex.hpp>
#include <hal/processor/gdt.hpp>
#include <hal/processor/interrupts/idt.hpp>
#include <hal/processor/cpuid.hpp>
#include <hal/processor/access.hpp>
#include <mt/mt.hpp>

namespace SMP
{
    cpu_data_t cpu_infos[SMP_MAX_CORES];
    unsigned int num_cpus = 0;
    unsigned int inited_cpus = 0;

    struct limine_smp_request smp_request = {
        .id = LIMINE_SMP_REQUEST,
        .revision = 0,
        .response = nullptr,
        .flags = LIMINE_SMP_X2APIC,
    };

    Spinlock iic_spinlock;

    void inc_inited_cpu()
    {
        iic_spinlock.lock();
        inited_cpus++;
        iic_spinlock.unlock();
    }

    extern "C" void enable_sse();

    void init_smp()
    {
        num_cpus = smp_request.response->cpu_count;

        for (auto& cpu : cpu_infos)
        {
            cpu.is_dispatched = false;
            cpu.processor_id = INVALID_CORE_ID;
        }

        if (smp_request.response == nullptr)
        {
            std::printf("SMP Request is null.\n");
        } else {
            std::printf("CPU Count: %d\n", smp_request.response->cpu_count); 
            
            for (unsigned int i = 0; smp_request.response->cpu_count > i; i++)
            {
                auto cpu = smp_request.response->cpus[i];
                
                if (cpu->lapic_id == smp_request.response->bsp_lapic_id)
                {
                    continue;
                }

                cpu->extra_argument = 1;
                cpu->goto_address = cpu_loop;
            }
        }

        // spin until all of them are init'ed.
        while (inited_cpus + 1 != smp_request.response->cpu_count)
        {
            __asm__ volatile("nop");
        }
    }

    cpu_data_t* get_cpu_data()
    {
        asm volatile ("swapgs");
        uint64_t msr = rdmsr(0xC0000101);
        asm volatile ("swapgs");

        return (cpu_data_t*)msr;
    }

    void cpu_loop(limine_smp_info* info)
    {
        cpu_infos[info->processor_id].is_dispatched = false;
        cpu_infos[info->processor_id].processor_id = info->processor_id;

        wrmsr(0xC0000101, (uint64_t)&cpu_infos[info->processor_id]);
        asm volatile ("swapgs");

        enable_sse();
        gdt_init();
        idt_init();
        // Multitasking::PerCoreInitialize(info->processor_id);

        std::printf("Hello! I'm CPU#%d! LAPIC ID: %llx\n", info->processor_id, info->lapic_id);
        
        inc_inited_cpu();

        while(true)
        {
            __asm__("nop");
        }
    }

    void print_cpus()
    {
        for (auto cpu : cpu_infos)
        {
            if (cpu.processor_id == INVALID_CORE_ID)
            {
                break;
            }

            std::printf("Processor ID: %d @@ Is Used: %d\n", cpu.processor_id, cpu.is_dispatched);
        }
    }

    uint64_t get_core_count()
    {
        return num_cpus;
    }
}