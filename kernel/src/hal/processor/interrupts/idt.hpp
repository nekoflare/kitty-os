#ifndef IDT_HPP
#define IDT_HPP

#include <stdint.h>
#include <hal/processor/smp/smp.hpp>

namespace IDT
{
    enum Attributes
    {
        PRESENT = 0b1000000000000000,
        RING0 = 0b0000000000000000,
        RING1 = 0b0010000000000000,
        RING2 = 0b0100000000000000,
        RING3 = 0b0110000000000000,
        INTERRUPT_GATE = 0b0000111000000000,
        TRAP_GATE = 0b0000111100000000,
    };
}

typedef struct __attribute__((packed))
{
    uint16_t size;
    uint64_t offset;
} idtr;

typedef struct __attribute__((packed))
{
    uint16_t offset0;
    uint16_t segment_sel;
    uint16_t attributes;
    uint16_t offset1;
    uint32_t offset2;
    uint32_t zero;
} gate_desc_64;

struct registers_t
{
    uint64_t user_gs, user_fs, user_es, user_ss, user_ds, user_cs;
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rdi, rsi, rbp, rdx, rcx, rbx, rax;
    uint64_t interrupt, error_code;
    uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed));

extern "C" void interrupt_handler(registers_t regs);

#define IDT_ENTRY(offset, selector, attributes) { \
    (uint16_t)((uint64_t)(offset) & 0xFFFF), \
    (uint16_t)(selector), \
    (uint16_t)(attributes), \
    (uint16_t)(((uint64_t)(offset) >> 16) & 0xFFFF), \
    (uint32_t)(((uint64_t)(offset) >> 32) & 0xFFFFFFFF), \
    0 \
}

__attribute__((aligned(0x10))) extern gate_desc_64 idt_entries[256];

void idt_init();
extern "C" void idt_load(idtr* idt);
extern "C" void isr_common();

void enable_interrupts();
void disable_interrupts();

#endif