#include <limine.h>
#include <graphics/graphics.hpp>
#include <error_codes.hpp>
#include <libc/stdio.hpp>
#include <mm/mm.hpp>
#include <hal/processor/smp/smp.hpp>
#include <hal/bus/pci.hpp>
#include <hal/processor/gdt.hpp>
#include <hal/processor/access.hpp>
#include <hal/processor/interrupts/idt.hpp>
#include <mt/mt.hpp>    
#include <exec/elf/elf.hpp>
#include <arch/amd64/halt.h>
#include <drivers/hwinf.h>
#include <serial/Serial.h>

LIMINE_BASE_REVISION(1)

// Halt and catch fire function.
static void hcf() {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

// Extern declarations for global constructors array.
extern void (*__init_array[])();
extern void (*__init_array_end[])();

limine_module_request limine_modules = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 0,
    .response = nullptr
};

extern driver_structure _driver_array;
extern driver_structure _driver_array_end;

extern "C" void kernel_main()
{
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    // Call global constructors.
    for (std::size_t i = 0; &__init_array[i] != __init_array_end; i++) {
        __init_array[i]();
    }

    // Enable all COM ports.

    auto qd_error = Graphics::QuickDraw::QDInit();
    if (qd_error != SUCCESS)
    {
        hcf();
    }

    std::init_stdio();

    //serial_printf("System modules: \n");
    for (uint i = 0; limine_modules.response->module_count > i; i++)
    {
        std::printf("Module name: %s Module Address: %p", limine_modules.response->modules[i]->cmdline, limine_modules.response->modules[i]->address);
        //serial_printf("\tModule name: %s Module address: %llx\n", limine_modules.response->modules[i]->cmdline, limine_modules.response->modules[i]->address);
    }

    gdt_init();
    idt_init();

    Memory::init();
    SMP::init_smp();

    SMP::cpu_infos[0].is_dispatched = true;
    SMP::cpu_infos[0].processor_id = 0;
    wrmsr(0xC0000101, (uint64_t)&SMP::cpu_infos[0]);
    asm volatile("swapgs");

    PCI::scanPCI();

    // Find hello_world
    /*for (uint32_t i = 0; limine_modules.response->module_count > i; i++)
    {
        auto& mod = limine_modules.response->modules[i];

        if (strcmp(mod->cmdline, "hello_world") == 0)
        {
            std::printf("Found the hello_world application!\n");

            //ELF::print_information_elf64(mod->address);

            return;
        }
    }*/

    Multitasking::PerCoreInitialize(0);


    uint64_t start = (uint64_t)&_driver_array;
    uint64_t end = (uint64_t)&_driver_array_end;
    uint64_t diff = end - start;
    uint64_t driver_count = diff / sizeof(driver_structure);

    std::printf("Driver count: %lld\n", driver_count);

    for (uint64_t i = 0; driver_count > i; i++)
    {
        uint64_t new_address = start + (i * sizeof(driver_structure));
        driver_structure* this_driver = (driver_structure*) new_address;

        std::printf("Driver name: %s\n", this_driver->_driver_info.driver_name);
    }

    // Iterate through drivers.

    halt(NO_REASON);

    // We're done, just hang...
    hcf();
}
