//
// Created by Piotr on 28.03.2024.
//

#include <iostream>
#include <fstream>
#include <sstream>

class IDT {
public:
    enum Attributes {
        PRESENT = 0b10000000,
        RING0 = 0b00000000,
        INTERRUPT_GATE = 0b1110
    };
};

class ISR {
public:
    ISR(int number) : number(number) {}
    int number;
};

std::string generate_idt_entry(const ISR& isr) {
    return "IDT_ENTRY(&ISR" + std::to_string(isr.number) + ", 0x8, IDT::Attributes::PRESENT | IDT::Attributes::RING0 | IDT::Attributes::INTERRUPT_GATE)";
}

void generate_idt_entries(int num_entries, const std::string& filename) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    for (int i = 0; i < num_entries; ++i) {
        ISR isr(i);
        outfile << generate_idt_entry(isr) << "," << std::endl;
    }

    outfile.close();
    std::cout << "IDT entries generated and saved to " << filename << std::endl;
}

int main() {
    int num_entries = 256;
    std::string filename = "idt_entries.txt";
    generate_idt_entries(num_entries, filename);
    return 0;
}
