#pragma once

#include <stdint.h>
#include <libc/stdio.hpp>

namespace ELF
{
    enum exec_types : uint16_t
    {
        RELOCATABLE = 1,
        EXECUTABLE = 2,
        SHARED = 3,
        CORE = 4,
    };

    enum instr_set_types : uint16_t
    {
        NO_SPECIFIC = 1,
        SPARC = 0x2,
        X86 = 0x3,
        MIPS = 0x8,
        POWERPC = 0x14,
        ARM = 0x28,
        SUPERH = 0x2a,
        IA64 = 0x32,
        X86_64 = 0x3e,
        AARCH64 = 0xb7,
        RISCV = 0xf3,
    };

    enum phdr_segment_type : uint32_t
    {
        IGNORE_NULL = 0,
        LOAD = 1,
        DYNAMIC = 2,
        INTERP = 3,
        NOTE_SECT = 4
    };

    struct phdr
    {
        phdr_segment_type segment_type;
        uint32_t flags;
        uint64_t p_offset;
        uint64_t p_vaddr;
        uint64_t undefined;
        uint64_t p_filesz;
        uint64_t p_memsz;
        uint64_t alignment;
    } __attribute__((packed));

    struct elf_hdr_64
    {
        uint8_t e_ident[4];
        uint8_t e_type;
        uint8_t e_endianness;
        uint8_t e_hdr_version;
        uint8_t e_os_abi;
        uint8_t e_unused[8];
        exec_types e_exec_type;
        instr_set_types e_instr_set;
        uint32_t e_version;
        uint64_t e_entry_point;
        uint64_t e_phdr_table_pos;
        uint64_t e_shdr_table_pos;
        uint32_t e_flags;
        uint16_t e_hdr_size;
        uint16_t e_phdr_entry_size;
        uint16_t e_phdr_entry_count;
        uint16_t e_shdr_entry_size;
        uint16_t e_shdr_entry_count;
        uint16_t e_section_idx_hdr;

    } __attribute__((packed));

    void print_information_elf64(void* elf_file);
}