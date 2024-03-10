#include <hal/processor/gdt.hpp>
#include <libc/stdio.hpp>

gdt_entry __attribute__((packed)) gdt[] = {
    GDT_ENTRY(0, 0, 0, 0),

    /* CODE */
    GDT_ENTRY(0, GRAN_4KIB | LONG_MODE, 0, PRESENT | RING0 | DESC_TYPE_REG | DESC_TYPE_CODE | READABLE_BIT),

    /* DATA */
    GDT_ENTRY(0, GRAN_4KIB | SIZE_FLAG, 0, PRESENT | RING0 | DESC_TYPE_REG | DESC_TYPE_DATA | READABLE_BIT),

    /* Empty TSS */
    GDT_ENTRY(0, 0, 0, 0),

    /* USER CODE */
    GDT_ENTRY(0, GRAN_4KIB | LONG_MODE, 0, PRESENT | RING3 | DESC_TYPE_REG | DESC_TYPE_CODE | READABLE_BIT),

    /* USER DATA */
    GDT_ENTRY(0, GRAN_4KIB | SIZE_FLAG, 0, PRESENT | RING3 | DESC_TYPE_REG | DESC_TYPE_DATA | READABLE_BIT),

    // TSS template
    //  I DONT KNOW WHAT IM DOING
    GDT_ENTRY(0, 0, 0, 0),
    GDT_ENTRY(0, 0, 0, 0),
};

gdtr_64 gdtr;

extern "C" void load_gdt(gdtr_64* _Gdt);

void gdt_init()
{
    gdtr.size = sizeof(gdt) - 1;
    gdtr.base_address = (uint64_t)&gdt;

    load_gdt(&gdtr);
}