def generate_idt_entries_hpp(filename, num_entries):
    with open(filename, 'w') as f:
        f.write('#ifndef IDT_ENTRIES_HPP\n')
        f.write('#define IDT_ENTRIES_HPP\n\n')
        f.write('#include "idt.hpp"\n\n')
        f.write('__attribute__((aligned(0x10)))\n')
        f.write('gate_desc_64 idt_entries[{}] = {{\n'.format(num_entries))
        for i in range(num_entries):
            f.write('\tIDT_ENTRY(&ISR{}, 0x8, IDT::Attributes::PRESENT | IDT::Attributes::RING0 | IDT::Attributes::INTERRUPT_GATE),\n'.format(i))
        f.write('};\n\n')
        f.write('#endif')

# Usage example
generate_idt_entries_hpp('idt_entries.hpp', 256)
