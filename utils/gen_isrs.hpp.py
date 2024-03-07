def generate_isrs_hpp(filename, num_entries):
    with open(filename, 'w') as f:
        f.write('#ifndef ISRS_HPP\n')
        f.write('#define ISRS_HPP\n\n')
        f.write('/* {} entries of extern defs */\n'.format(num_entries))
        for i in range(num_entries):
            f.write('extern "C" void ISR{}();\n'.format(i))
        f.write('\n#endif')

# Usage example
generate_isrs_hpp('isrs.hpp', 256)
