//
// Created by Piotr on 24.03.2024.
//

#include <cstdint>
#include "gdt.h"

static volatile HAL::gdt_entry gdt[] = {
        GDT_ENTRY(0, 0, 0, 0),

        /* CODE */
        GDT_ENTRY(0, HAL::GRAN_4KIB | HAL::LONG_MODE, 0, HAL::PRESENT | HAL::RING0 | HAL::DESC_TYPE_REG | HAL::DESC_TYPE_CODE | HAL::READABLE_BIT),

        /* DATA */
        GDT_ENTRY(0, HAL::GRAN_4KIB | HAL::SIZE_FLAG, 0, HAL::PRESENT | HAL::RING0 | HAL::DESC_TYPE_REG | HAL::DESC_TYPE_DATA | HAL::READABLE_BIT),

        /* Empty TSS */
        GDT_ENTRY(0, 0, 0, 0),

        /* USER CODE */
        GDT_ENTRY(0, HAL::GRAN_4KIB | HAL::LONG_MODE, 0, HAL::PRESENT | HAL::RING3 | HAL::DESC_TYPE_REG | HAL::DESC_TYPE_CODE | HAL::READABLE_BIT),

        /* USER DATA */
        GDT_ENTRY(0, HAL::GRAN_4KIB | HAL::SIZE_FLAG, 0, HAL::PRESENT | HAL::RING3 | HAL::DESC_TYPE_REG | HAL::DESC_TYPE_DATA | HAL::READABLE_BIT),

        // TSS template
        GDT_ENTRY(0, 0, 0, 0),
        GDT_ENTRY(0, 0, 0, 0),
};

void HAL::x86_init_gdt()
{
    gdtr_64 gdtr;
    gdtr.base_address = reinterpret_cast<std::uint64_t>(&gdt);
    gdtr.size = sizeof(gdt) - 1;

    x86_load_gdt(&gdtr);
}