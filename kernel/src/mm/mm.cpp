#include <mm/mm.hpp>
#include <limine.h>
#include <libc/stdio.hpp>
#include <drivers/serial/serial.hpp>

namespace Memory
{
    u64* pml4 = nullptr;

    u64 limine_memmap_entry_count = 0;
    limine_memmap_entry** limine_memmap_entries = nullptr;

    u64 mm_total_memory = 0;
    u64 mm_usable_memory = 0;
    u64 mm_unusable_memory = 0;
    u64 mm_bad_memory = 0;

    u8* bitmap = nullptr;

    limine_memmap_request _limine_memmap_request = {
        .id = LIMINE_MEMMAP_REQUEST,
        .revision = 0
    };

    limine_hhdm_request _limine_hhdm_request = {
        .id = LIMINE_HHDM_REQUEST,
        .revision = 0
    };

    limine_memmap_response* _limine_memmap_response = nullptr; // Define and initialize
    limine_hhdm_response* _limine_hhdm_response = nullptr; // Define and initialize
}

void* Memory::get_pml4()
{
    uint64_t hhdm_offset = Memory::_limine_hhdm_response->offset;

    uint64_t _cr3 = Memory::_x64_read_cr3();
    uint64_t pml4 = _cr3 & 0xFFFFFFFFFFFFF000;

    pml4 += hhdm_offset;

    return (void*) pml4;
}

uint64_t Memory::get_virtual_address(uint64_t paddr)
{
    return paddr + Memory::_limine_hhdm_response->offset;
}

void Memory::init()
{
    // Checking for null pointers.
    // TODO: Add logging.
    if (Memory::_limine_memmap_request.response == nullptr ||
        Memory::_limine_hhdm_request.response == nullptr 
    )
    {
        _imm_halt();

        __builtin_unreachable();
    }

    // Initial variable setting up.
    Memory::_limine_memmap_response = Memory::_limine_memmap_request.response;
    Memory::limine_memmap_entries = Memory::_limine_memmap_request.response->entries;
    Memory::limine_memmap_entry_count = Memory::_limine_memmap_request.response->entry_count;
    Memory::_limine_hhdm_response = Memory::_limine_hhdm_request.response;

    // Get PML4 and load it to variable.
    Memory::pml4 = (u64*) Memory::get_pml4();

    for (u64 i = 0; Memory::limine_memmap_entry_count > i; i++)
    {
        Memory::mm_total_memory += Memory::limine_memmap_entries[i]->length;

        switch (Memory::limine_memmap_entries[i]->type)
        {
            case LIMINE_MEMMAP_USABLE:
                Memory::mm_usable_memory += Memory::limine_memmap_entries[i]->length;
                break;
            case LIMINE_MEMMAP_RESERVED:
                Memory::mm_unusable_memory += Memory::limine_memmap_entries[i]->length;
                break;
            case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
                Memory::mm_usable_memory += Memory::limine_memmap_entries[i]->length;
                break;
            case LIMINE_MEMMAP_ACPI_NVS:
                Memory::mm_unusable_memory += Memory::limine_memmap_entries[i]->length;
                break;
            case LIMINE_MEMMAP_BAD_MEMORY:
                Memory::mm_bad_memory += Memory::limine_memmap_entries[i]->length;
                break;
            case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
                Memory::mm_unusable_memory += Memory::limine_memmap_entries[i]->length;
                break;
            case LIMINE_MEMMAP_FRAMEBUFFER:
                Memory::mm_unusable_memory += Memory::limine_memmap_entries[i]->length;
                break;
            default:
                Memory::mm_bad_memory += Memory::limine_memmap_entries[i]->length;
                break;
        }
    }

    // Allocate space for the bitmap.
    // Divide total memory by page size (4096 bytes) and byte 8 because that's how much bits are in the byte
    // and we're storing 8 pages in one byte.
    // In the end align it to page(-s).
    // Calculate the number of pages needed for the bitmap
    u64 bytes_for_bitmap = (mm_total_memory / PAGE_SIZE / 8);
    bytes_for_bitmap = (bytes_for_bitmap + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1); // Align to page size
    std::printf("Bytes for bitmap (KB): %lld\n", bytes_for_bitmap / 1024);
    std::printf("Pages for bitmap: %lld\n", bytes_for_bitmap / PAGE_SIZE);

    // Find in the memory map the most suitable memory block.
    for (unsigned int i = 0; limine_memmap_entry_count > i; i++)
    {
        limine_memmap_entry* entry = limine_memmap_entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            // Check its' size.
            if (entry->length >= bytes_for_bitmap)
            {
                // Good size. 
                std::printf("Found address: %llx with size of %llx that is good for bitmap.\n", entry->base, entry->length);

                uint64_t phys_address = entry->base;

                // Manipulate that entry.
                entry->base += bytes_for_bitmap;
                entry->length -= bytes_for_bitmap;

                // Get virtual address for the bitmap.
                uint64_t virt_address = Memory::get_virtual_address(phys_address);

                // Show the bitmap.
                std::printf("Here's the address of bitmap: %llx\n", virt_address);

                bitmap = (u8*)virt_address;
                break;
            }
        }
    }

    if (bitmap != nullptr)
    {
        // Fill it with ones.
        for (u64 i = 0; bytes_for_bitmap > i; i++)
        {
            bitmap[i] = 0xFF;
        }
    }

    for (u64 i = 0; Memory::limine_memmap_entry_count > i; i++)
    {
        limine_memmap_entry* mmap_entry = Memory::limine_memmap_entries[i];
        switch (Memory::limine_memmap_entries[i]->type)
        {
            case LIMINE_MEMMAP_USABLE:
                // Mark this in bitmap as usable (0).
                mark_pages_in_range(bitmap, mmap_entry->base, mmap_entry->base + mmap_entry->length, true);
                break;
        }
    }

    // I'd like to keep bootloader data and ACPI in case I wanna do something cool in the future.
    
    std::printf("Available memory (KB): %lld\n", mm_usable_memory / 1024);
    std::printf("Total memory (KB): %lld\n", mm_total_memory / 1024);
}

void Memory::mark_page(u8* bitmap, u64 page)
{
    u64 byte_index = page / 8;
    u64 bit_index = page % 8;

    bitmap[byte_index] |= (1 << bit_index);
}

void Memory::unmark_page(u8* bitmap, u64 page)
{
    u64 byte_index = page / 8;
    u64 bit_index = page % 8;   

    bitmap[byte_index] &= ~(1 << bit_index);
}

void Memory::mark_pages_in_range(u8* bitmap, u64 start_addr, u64 end_addr, bool unmark)
{
    u64 start_page = start_addr / PAGE_SIZE;
    u64 end_page = end_addr / PAGE_SIZE;

    if (unmark)
    {
        std::printf("[OK] Marking as usable %llx -> %llx\n", start_addr, end_addr);

        for (u64 page = start_page; page <= end_page; page++)
        {
            u64 byte_index = page / 8;
            u64 bit_index = page % 8;

            bitmap[byte_index] &= ~(1 << bit_index);
        }
    }
    else 
    {
        std::printf("[OK] Marking as unusable %llx -> %llx\n", start_addr, end_addr);

        for (u64 page = start_page; page <= end_page; page++)
        {
            u64 byte_index = page / 8;
            u64 bit_index = page % 8;

            bitmap[byte_index] |= (1 << bit_index);
        }
    }

    return;
}

bool Memory::is_page_available(u64 page) {
    u64 byte_index = page / 8;
    u64 bit_index = page % 8;

    return (bitmap[byte_index] & (1 << bit_index)) == 0;
}

u64 Memory::page_to_physical_address(u64 page)
{
    return page * PAGE_SIZE;
}

void* Memory::alloc_page()
{
    // find continous block of memory and return virtual address of it.
    bool found_block = false;
    u64 page = 0;
    while (!found_block)
    {
        if (Memory::is_page_available(page))
        {
            Memory::mark_page(bitmap, page);
            std::printf("Page: %lld\n", page);
            // return address of that page.
            return (void*)Memory::page_to_physical_address(page);
        }

        page++;
    }

    return (void*)0xFFFFFFFFFFFFFFFF;
}

void Memory::free_page(u64 page)
{
    Memory::unmark_page(bitmap, page);
}

void* Memory::malloc(u64 len, u64 virt_address, u64 flags)
{
    // Alright, we gotta know how much memory we should allocate in pages.
    u64 pages = (len + PAGE_SIZE - 1) / PAGE_SIZE;
    std::printf("[INFO] Trying to allocate %lld pages.\n", pages);

    
}