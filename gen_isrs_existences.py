# Generate ISR declarations and store them in a list
isr_declarations = [f"extern void isr{i}();" for i in range(256)]

# Write the declarations to a C file
with open('isr_declarations.c', 'w') as file:
    file.write('#ifndef ISR_DECLARATIONS_H\n')
    file.write('#define ISR_DECLARATIONS_H\n\n')
    file.write('// ISR declarations\n')
    file.write('\n'.join(isr_declarations))
    file.write('\n\n#endif // ISR_DECLARATIONS_H')
