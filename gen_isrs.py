# Define the template for IDT descriptor
descriptor_template = """idt_set_descriptor(0x{:02X}, isr{}, IDT_FLAG_PRESENT | 0xE | IDT_FLAG_RING0);"""

# Generate IRQ definitions and store them in a list
irq_definitions = [descriptor_template.format(i, i) for i in range(256)]

# Write the definitions to a C file
with open('irqs.c', 'w') as file:
    file.write('#include "idt.h"\n\n')  # Include necessary header file
    file.write('void setup_irqs() {\n')
    file.write('\n'.join(irq_definitions))
    file.write('\n}')
