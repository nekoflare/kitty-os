#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#pragma pack(push, 1)
struct BMPHeader {
    char header[2];
    unsigned int fileSize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int dataOffset;
};

struct DIBHeader {
    unsigned int dibHeaderSize;
    int width;
    int height;
    unsigned short colorPlanes;
    unsigned short bitsPerPixel;
    unsigned int compressionMethod;
    unsigned int imageSize;
    int horizontalResolution;
    int verticalResolution;
    unsigned int colorsInPalette;
    unsigned int importantColors;
};
#pragma pack(pop)

vector<char> decompressBMP(const char* filename) {
    vector<char> decompressedData;

    ifstream file(filename, ios::binary | ios::ate);
    if (!file.is_open()) {
        cerr << "Error opening file " << filename << endl;
        return decompressedData;
    }

    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    BMPHeader bmpHeader;
    DIBHeader dibHeader;

    file.read(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPHeader));
    file.read(reinterpret_cast<char*>(&dibHeader), sizeof(DIBHeader));

    // Check if BMP is uncompressed
    if (dibHeader.compressionMethod != 0) {
        cerr << "Error: Only uncompressed BMP files are supported." << endl;
        return decompressedData;
    }

    // Read image data
    file.seekg(bmpHeader.dataOffset, ios::beg);
    decompressedData.resize(size - bmpHeader.dataOffset);
    file.read(decompressedData.data(), size - bmpHeader.dataOffset);

    file.close();

    return decompressedData;
}

void saveAsBMP(const char* filename, const vector<char>& imageData) {
    ofstream outFile(filename, ios::binary);
    if (!outFile.is_open()) {
        cerr << "Error creating file " << filename << endl;
        return;
    }

    outFile.write(imageData.data(), imageData.size());

    outFile.close();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " input_file output_file" << endl;
        return 1;
    }

    const char* inputFilename = argv[1];
    const char* outputFilename = argv[2];

    vector<char> decompressedData = decompressBMP(inputFilename);
    if (decompressedData.empty()) {
        cerr << "Error decompressing BMP file." << endl;
        return 1;
    }

    saveAsBMP(outputFilename, decompressedData);
    cout << "Decompressed BMP file saved as " << outputFilename << endl;

    return 0;
}
    