#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>

struct BMPHeader {
    uint16_t signature;
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
    uint32_t headerSize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    // Other fields can be added as needed
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename.bmp>" << std::endl;
        return 1;
    }

    const char* filename = argv[1];

    // Open the BMP file
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return 1;
    }

    // Read the BMP header
    BMPHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    // Check if the BMP file is compressed
    bool isCompressed = (header.compression != 0);

    // Output the result
    if (isCompressed) {
        std::cout << "The BMP file " << filename << " is compressed." << std::endl;
    } else {
        std::cout << "The BMP file " << filename << " is not compressed." << std::endl;
    }

    return 0;
}
