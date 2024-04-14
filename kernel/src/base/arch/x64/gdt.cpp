//
// Created by Piotr on 14.04.2024.
//

#include "gdt.h"

static GDTEntry gdt[] = {
        GDT_ENTRY(0, 0, 0, 0), // NULL entry
        GDT_ENTRY(0, GDT_GRAN_4KIB | GDT_LONG_MODE, 0, GDT_PRESENT | GDT_RING0 | GDT_DESC_TYPE_REG | GDT_DESC_TYPE_CODE | GDT_READABLE_BIT),
        GDT_ENTRY(0, GDT_GRAN_4KIB | GDT_SIZE_FLAG, 0, GDT_PRESENT | GDT_RING0 | GDT_DESC_TYPE_REG | GDT_DESC_TYPE_DATA | GDT_READABLE_BIT),

        // Creating space for TSS.
        GDT_ENTRY(0, 0, 0, 0),
        GDT_ENTRY(0, 0, 0, 0),

        // User
        GDT_ENTRY(0, GDT_GRAN_4KIB | GDT_LONG_MODE, 0, GDT_PRESENT | GDT_RING3 | GDT_DESC_TYPE_REG | GDT_DESC_TYPE_CODE | GDT_READABLE_BIT),
        GDT_ENTRY(0, GDT_GRAN_4KIB | GDT_SIZE_FLAG, 0, GDT_PRESENT | GDT_RING3 | GDT_DESC_TYPE_REG | GDT_DESC_TYPE_DATA | GDT_READABLE_BIT),
};

void KeFlushGlobalDescriptorTable()
{
    static GDTRegister64 gdtr = {
            .size = sizeof(gdt) - 1,
            .address = reinterpret_cast<std::uint64_t>(&gdt)
    };

    KeASMReloadSegments(&gdtr);
}