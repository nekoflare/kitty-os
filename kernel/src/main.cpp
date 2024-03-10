#include <limine.h>
#include <hal/processor/extensions/sse.hpp>
#include <hal/processor/io.hpp>
#include <kern_api/kern_api.hpp>
#include <libc/string.hpp>
#include <graphics/graphics.hpp>
#include <error_codes.hpp>
#include <flanterm/flanterm.h>
#include <flanterm/backends/fb.h>
#include <libc/stdio.hpp>
#include <hal/processor/speed.hpp>
#include <math/math.hpp>
#include <mm/mm.hpp>
#include <hal/processor/smp/smp.hpp>
#include <power/power.hpp>
#include <hal/bus/pci.hpp>
#include <hal/processor/gdt.hpp>
#include <hal/processor/cpuid.hpp>
#include <hal/processor/access.hpp>
#include <hal/processor/interrupts/idt.hpp>
#include <acpi/acpi.hpp>
#include <mt/mt.hpp>
#include <kern_registry.hpp>
#include <exec/elf/elf.hpp>

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

void tick()
{
    std::printf(".");
}

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

    auto qd_error = Graphics::QuickDraw::QDInit();
    if (qd_error != SUCCESS)
    {
        hcf();
    }

    std::init_stdio();

    for (uint i = 0; limine_modules.response->module_count > i; i++)
    {
        std::printf("Module name: %s Module Address: %p", limine_modules.response->modules[i]->cmdline, limine_modules.response->modules[i]->address);
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
    for (uint32_t i = 0; limine_modules.response->module_count > i; i++)
    {
        auto& mod = limine_modules.response->modules[i];

        if (strcmp(mod->cmdline, "hello_world") == 0)
        {
            std::printf("Found the hello_world application!\n");

            ELF::print_information_elf64(mod->address);

            return;
        }
    }

    // Multitasking::PerCoreInitialize(0);

    std::printf("The address: %llx\n", (void*)&limine_modules);
    // We're done, just hang...
    hcf();
}
