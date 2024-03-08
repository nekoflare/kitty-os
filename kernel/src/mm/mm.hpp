#ifndef MM_HPP
#define MM_HPP

#include <limine.h>
#include <types.h>

namespace Memory
{
    static void _imm_halt()
    {
        asm volatile("cli");
        for (;;)
        {
            asm volatile("hlt");
        }
    }

    static void _invlpg(void* vaddr)
    {
        asm volatile("invlpg (%0)" :: "r"(vaddr) : "memory");
    }

    static u64 _x64_read_cr3()
    {
        uint64_t _cr3;
        asm volatile("mov %%cr3, %0" : "=r"(_cr3));
        return _cr3;
    }

    #define PAGE_SIZE 4096

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

    extern u64 mm_total_memory;
    extern u64 mm_usable_memory;
    extern u64 mm_unusable_memory;
    extern u64 mm_bad_memory;

    extern u64* pml4;
    extern u8* bitmap;

    extern limine_memmap_request _limine_memmap_request;
    extern limine_memmap_response* _limine_memmap_response;
    extern limine_hhdm_request _limine_hhdm_request;
    extern limine_hhdm_response* _limine_hhdm_response;

    extern limine_memmap_entry** limine_memmap_entries;
    extern u64 limine_memmap_entry_count;

    void init();
    void* get_pml4();
    uint64_t get_virtual_address(uint64_t paddr);

    // Bitmap manipulation.
    void mark_page(u8* bitmap, uint64_t page_num);
    void unmark_page(u8* bitmap, uint64_t page_num);
    void mark_pages_in_range(u8* bitmap, uint64_t start_address, uint64_t end_address, bool is_zero);
    bool is_page_available(u64 page);

    // Virtual address manipulations and representations.
    u64 page_to_physical_address(u64 page);

    // Allocating.
    void* alloc_page();
    void free_page(u64 page);
    void* malloc(u64 len, u64 virt_address, u64 flags);

    enum PageAttributes
    {
        PRESENT = 1 << 0,
        READ_WRITE = 1 << 1,
        SUPERVISOR = 1 << 2,
        WRITE_THROUGH = 1 << 3,
        CACHE_DISABLE = 1 << 4,
        ACCESSED = 1 << 5,
        DIRTY = 1 << 6,
        PAGE_ATTRIBUTE_TABLE = 1 << 7,
        GLOBAL = 1 << 8,
    };
};

#endif