#include <libc/stdio.hpp>
#include <exec/elf/elf.hpp>

namespace ELF
{
    const char* ELF_MAGIC = "\x7f" "ELF";
    const char ELF32_TYPE = 1;
    const char ELF64_TYPE = 2;

    void print_information_elf64(void* elf_file = nullptr)
    {
        if (elf_file == nullptr)
        {
            std::printf("[ELF] No file specified.\n");

            return;
        }

        elf_hdr_64* e64_hdr = (elf_hdr_64*)elf_file;

        std::printf("[ELF] OS ABI: %llx\n", (uint64_t)e64_hdr->e_os_abi);
        std::printf("[ELF] Entry point: %llx\n", e64_hdr->e_entry_point);
        std::printf("[ELF] e_hdr_size=%llx\n", (uint64_t)e64_hdr->e_hdr_size);
        std::printf("[ELF] e_phdr_entry_size=%lld\n", (uint64_t)e64_hdr->e_phdr_entry_size);
        std::printf("[ELF] e_phdr_entry_count=%lld\n", (uint64_t)e64_hdr->e_phdr_entry_count);
        std::printf("[ELF] e_version=%llx\n", (uint64_t)e64_hdr->e_version);
        std::printf("[ELF] e_phdr_table_pos=%lld\n", (uint64_t)e64_hdr->e_phdr_table_pos);
        std::printf("[ELF] e_shdr_table_pos=%lld\n", (uint64_t)e64_hdr->e_shdr_table_pos);

        phdr* phdrs = (phdr*)((uint64_t)elf_file + e64_hdr->e_phdr_table_pos);

        for (unsigned int i = 0; e64_hdr->e_phdr_entry_size > i; i++)
        {
            std::printf("ENTRY %d\n", i);
            std::printf("[ELF] Segment type: %d\n", phdrs->segment_type);
            std::printf("[ELF] Flags: %x\n", phdrs->flags);
            std::printf("[ELF] p_offset = %llx\n", phdrs->p_offset);
            std::printf("[ELF] p_vaddr = %llx\n", phdrs->p_vaddr);
            std::printf("[ELF] p_filesz = %llx\n", phdrs->p_filesz);
            std::printf("[ELF] p_memsz = %llx\n", phdrs->p_memsz);
            std::printf("[ELF] alignment = %llx\n\n\n", phdrs->alignment);
            phdrs += e64_hdr->e_hdr_size;
        }

        

        return;
    }
}