#include <mm/mm.hpp>
#include <libc/stdio.hpp>
#include <limine.h>

#define PAGE_SIZE 4096

uint64_t overall_memory = 0;
uint64_t usable_memory = 0;
uint64_t bad_memory = 0;
uint64_t unusable_memory = 0;

limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
    .response = nullptr
};

limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
    .response = nullptr
};

limine_kernel_address_request kernel_address_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0,
    .response = nullptr
};

volatile limine_memmap_response* memory_map_response = nullptr;
limine_memmap_entry** mmap_entries = nullptr;
volatile uint64_t mmap_entry_count = 0;

static void hcf() {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

typedef unsigned long long u64;

static volatile void _invlpg(void* addr)
{
    asm volatile("invlpg (%0)" :: "r"(addr) : "memory");
}

/*
    Structure of PML4:
    63 - eXecute Disable
    62 .. 52 - Available
    51 .. M - Reserved (0)
    M-1 .. 12 - Address
    11 .. 8 - Available
    7 - Reserved (0)
    6 - Available
    5 - Accessed
    4 - Cache Disable
    3 - Write-Through
    2 - User/Supervisor
    1 - Read/Write
    0 - Present
*/

// #define PML4_ENTRY(execute_disable, )

u64 pml4_entries[512];

uint64_t read_cr3()
{
    uint64_t cr3;
    asm volatile ("mov %%cr3, %0" : "=r" (cr3));
    return cr3;
}

void* get_page_dir_base()
{
    uint64_t hhdm_offset = hhdm_request.response->offset;
    //uint64_t paddr = (uint64_t)vaddr - hhdm_offset;
    //return paddr;

    uint64_t cr3 = read_cr3();
    uint64_t page_dir_base = cr3 &  0xFFFFFFFFFFFFF000;

    page_dir_base += hhdm_offset;

    //std::printf("Page dir base: %llx\n", page_dir_base);
    //std::printf("First entry of PML4 of Limine: %llx\n", ((uint64_t*)page_dir_base)[0x1ff]);

    return (void*)page_dir_base;
}

void* get_virt_addr(void* paddr)
{
    return paddr + hhdm_request.response->offset;
}

// (8 * 1024 * 1024) / 8
u64 pm_bitmap[1048576]; // let's support 256GB of bitmap. it's just 8MB.

union vaddr_t
{
    uint64_t vaddr;
    struct vaddr_parts
    {
        uint64_t offset : 12;
        uint64_t table : 9;
        uint64_t directory : 9;
        uint64_t directory_ptr : 9;
        uint64_t pml4 : 9;
        uint64_t unused : 16;
    };
};

void mark_used_page_pm_bitmap(uint64_t page)
{
    // get the byte offset and bit. and flip it to 1.
    uint64_t byte_index = page / 64;
    uint64_t bit_offset = page % 64;

    pm_bitmap[byte_index] |= (1ULL << bit_offset);
}

void unmark_page_pm_bitmap(uint64_t page)
{
    // Calculate the index of the byte in pm_bitmap array
    uint64_t byte_index = page / 64;
    
    // Calculate the bit within the byte
    uint64_t bit_offset = page % 64;
    
    // Clear the corresponding bit to 0
    pm_bitmap[byte_index] &= ~(1UL << bit_offset);
}

uint64_t addr_to_page(void* addr)
{
    uint64_t uaddr = (uint64_t)addr;
    return (uaddr / 4096);
}

void mark_pages_in_range(uint64_t start_address, uint64_t end_address) {
    uint64_t page_size_bytes = 4096;
    
    // Iterate through the address range
    for (uint64_t address = start_address; address <= end_address; address += page_size_bytes) {
        // Convert address to page number
        uint64_t page_number = address / page_size_bytes;
        
        // Calculate the index of the byte in pm_bitmap array
        uint64_t byte_index = page_number / 64;
        
        // Calculate the bit within the byte
        uint64_t bit_offset = page_number % 64;
        
        // Set the corresponding bit to 1
        pm_bitmap[byte_index] |= (1UL << bit_offset);
    }
}

u64* pml4 = nullptr;

bool page_exists(void* addr)
{
    vaddr_t vaddr;
    uint64_t _vaddr = (uint64_t)addr;
    vaddr.vaddr = _vaddr;

    u64* dir = pml4[vaddr.vaddr_parts.directory_ptr];

    return true;
}


void Memory::Init()
{
    memory_map_response = memmap_request.response;
    mmap_entry_count = memmap_request.response->entry_count;
    mmap_entries = memmap_request.response->entries;

    for (auto &i : pm_bitmap)
    {
        i = 0;
    }

    if (hhdm_request.response != nullptr)
    {
        std::printf("HHDM offset: %llX\n", hhdm_request.response->offset);
    }

    if (kernel_address_request.response == nullptr)
    {
        hcf();
    }

    pml4 = (u64*)get_page_dir_base();

    std::printf("Kernel Physical Base: %p\nKernel Virtual Base: %p\n", kernel_address_request.response->physical_base, kernel_address_request.response->virtual_base);

    for (unsigned int i = 0; mmap_entry_count > i; i++)
    {
        overall_memory += mmap_entries[i]->length;
        switch (mmap_entries[i]->type)
        {
            case LIMINE_MEMMAP_USABLE:
                usable_memory += mmap_entries[i]->length;
                break;
            case LIMINE_MEMMAP_RESERVED:
                unusable_memory += mmap_entries[i]->length;
                // MARK AS 1
                mark_pages_in_range((uint64_t)(mmap_entries[i]->base), (uint64_t)(mmap_entries[i]->base + mmap_entries[i]->length));
                break;
            case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
                usable_memory += mmap_entries[i]->length;
                break;
            case LIMINE_MEMMAP_ACPI_NVS:
                unusable_memory += mmap_entries[i]->length;
                mark_pages_in_range((uint64_t)(mmap_entries[i]->base), (uint64_t)(mmap_entries[i]->base + mmap_entries[i]->length));
                break;
            case LIMINE_MEMMAP_BAD_MEMORY:
                bad_memory += mmap_entries[i]->length;
                mark_pages_in_range((uint64_t)(mmap_entries[i]->base), (uint64_t)(mmap_entries[i]->base + mmap_entries[i]->length));
                break;
            case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
                usable_memory += mmap_entries[i]->length;
                break;
            case LIMINE_MEMMAP_KERNEL_AND_MODULES:
                unusable_memory += mmap_entries[i]->length;
                mark_pages_in_range((uint64_t)(mmap_entries[i]->base), (uint64_t)(mmap_entries[i]->base + mmap_entries[i]->length));
                break;
            case LIMINE_MEMMAP_FRAMEBUFFER:
                unusable_memory += mmap_entries[i]->length;
                mark_pages_in_range((uint64_t)(mmap_entries[i]->base), (uint64_t)(mmap_entries[i]->base + mmap_entries[i]->length));
                break;
            default:
                std::printf("Unknown memory map entry type: %d\nMarking it as bad memory.\n", mmap_entries[i]->type);
                bad_memory += mmap_entries[i]->length;
                mark_pages_in_range((uint64_t)(mmap_entries[i]->base), (uint64_t)(mmap_entries[i]->base + mmap_entries[i]->length));
                break;
        }
        // std::printf("Base: %p Length: %llx Type: %s\n", mmap_entries[i]->base, mmap_entries[i]->length, TypeToString(mmap_entries[i]->type));
    }

    for (unsigned int i = 0; 512 > i; i++)
    {
        std::printf("%llx", pm_bitmap[i]);
    }

    // for (;;);

    // Gotta do paging now...

    //pt[0] |= (PageAttributes::PRESENT | PageAttributes::GLOBAL | PageAttributes::DIRTY | PageAttributes::READ_WRITE);
    // get dem address.
    //uint64_t phys_base = kernel_address_request.response->physical_base;

}

void print_virt_addr_as_str(void* addr)
{
    uint64_t u64addr = (uint64_t)addr;

    // How an address look like:
    // 0-11: Offset
    // 12-20: Table
    // 21-29: Dir
    // 30-38: Dir ptr
    // 39-47: PML4
    // Rest is discarded.

    uint64_t offset = u64addr & 0xFFF;          // Last 12 bits
    uint64_t table = (u64addr >> 12) & 0x1FF;   // Next 9 bits
    uint64_t dir = (u64addr >> 21) & 0x1FF;     // Next 9 bits
    uint64_t dirptr = (u64addr >> 30) & 0x1FF;  // Next 9 bits
    uint64_t pml4 = (u64addr >> 39) & 0x1FF;    // Next 9 bits

    std::printf("PML4: %llx DIRPTR: %llx DIR: %llx TABLE: %llx OFFSET: %llx", pml4, dirptr, dir, table, offset);

}

static const char* TypeToString(uint64_t type)
{
    switch (type)
    {
        case LIMINE_MEMMAP_USABLE:
            return "Usable RAM";
        case LIMINE_MEMMAP_RESERVED:
            return "Reserved - Unusable";
        case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
            return "ACPI Reclaimable memory";
        case LIMINE_MEMMAP_ACPI_NVS:
            return "ACPI NVS memory";
        case LIMINE_MEMMAP_BAD_MEMORY:
            return "Area containing bad memory";
        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
            return "Limine Bootloader (Reclaimable)";
        case LIMINE_MEMMAP_KERNEL_AND_MODULES:
            return "Kernel and Modules";
        case LIMINE_MEMMAP_FRAMEBUFFER:
            return "Framebuffer";
        default:
            return "Unknown memory area.";
    }
}

void Memory::PrintMemoryEntries()
{
    for (unsigned int i = 0; mmap_entry_count > i; i++)
    {
        std::printf("Base: %p Length: %llx Type: %s\n", mmap_entries[i]->base, mmap_entries[i]->length, TypeToString(mmap_entries[i]->type));
    }

    std::printf("Overall memory:(MB): %f\n", static_cast<float>(overall_memory) / 1024.0f / 1024.0f);
    std::printf("Bad memory (MB): %f\n", static_cast<float>(bad_memory) / 1024.0f / 1024.0f);
    std::printf("Usable memory (MB): %f\n", static_cast<float>(usable_memory) / 1024.0f / 1024.0f);
    std::printf("Unusable memory (MB): %f\n", static_cast<float>(unusable_memory) / 1024.0f / 1024.0f);
    std::printf("Usable memory out of overall: (%%): %f%%\n", (static_cast<float>(usable_memory) / static_cast<float>(overall_memory)) * 100.0f);

    uint64_t num_bytes_for_bitmap = (overall_memory / PAGE_SIZE) / 8;
    std::printf("Bytes needed for bitmap: %d\n", num_bytes_for_bitmap);

    // Find contigous block of memory to hold the bitmap.

    for (unsigned int i = 0; mmap_entry_count > i; i++)
    {
        if (mmap_entries[i]->length < num_bytes_for_bitmap) {
            continue; // Skip if the memory block is too small
        }

        if (mmap_entries[i]->type != LIMINE_MEMMAP_USABLE && mmap_entries[i]->type != LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) {
            continue; // Skip if the memory block is not usable or bootloader reclaimable
        }        

        std::printf("Found suitable memory block for bitmap: %llx\n", mmap_entries[i]->base);
    }

    get_page_dir_base();

}