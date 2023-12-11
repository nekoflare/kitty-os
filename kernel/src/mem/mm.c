#include "io/serial.h"
#include "mm.h"

struct KernelMemoryInformation kmi = {0};

struct limine_memmap_request mmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

const char* type_to_string(
    unsigned int type
)
{
    switch (type)
    {
        case LIMINE_MEMMAP_USABLE:
            return "MMAP-USABLE";
        case LIMINE_MEMMAP_RESERVED:
            return "MMAP-RESERVED";
        case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
            return "MMAP-ACPI-RECLAIMABLE";
        case LIMINE_MEMMAP_ACPI_NVS:
            return "MMAP-ACPI_NVS";
        case LIMINE_MEMMAP_BAD_MEMORY:
            return "MMAP-BAD-MEMORY";
        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
            return "MMAP-BTLDR-RECLAIMABLE";
        case LIMINE_MEMMAP_KERNEL_AND_MODULES:
            return "MMAP-KERN-AND-MODULES";
        case LIMINE_MEMMAP_FRAMEBUFFER:
            return "MMAP-FRAMEBUFFER";
        default:
            serial_printf("Unknown mmap %d.", type);
            return "MMAP-UNKNOWN";
    }
}

void initialize_mm()
{
    if (mmap_request.response == NULL)
    {
        serial_printf("Failed to start the OS. Failed to get the mmap.\n");
        return;
    }

    for (unsigned int i = 0; mmap_request.response->entry_count > i; i++)
    {
        serial_printf("TYPE: %s BASE: %x LEN: %x\n", type_to_string(mmap_request.response->entries[i]->type), mmap_request.response->entries[i]->base, mmap_request.response->entries[i]->length);
    }

    for (unsigned int i = 0; mmap_request.response->entry_count > i; i++)
    {
        kmi.memory_overall += mmap_request.response->entries[i]->length;

        switch(mmap_request.response->entries[i]->type)
        {
            case LIMINE_MEMMAP_USABLE:
            case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
            case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
                kmi.usable_memory += mmap_request.response->entries[i]->length;
                break;
            default:
                kmi.unusable_memory += mmap_request.response->entries[i]->length;
                break;
        }
    }
}

struct KernelMemoryInformation* get_kmi()
{
    return &kmi;
}