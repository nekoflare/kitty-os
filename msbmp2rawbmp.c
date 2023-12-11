#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)
typedef struct {
    unsigned short int type;           // Magic identifier (0x4D42)
    unsigned int size;                 // File size in bytes
    unsigned short int reserved1, reserved2;
    unsigned int offset;               // Offset to image data in bytes
} BMPHeader;

typedef struct {
    unsigned int size;                 // Header size in bytes
    int width, height;                 // Width and height of the image
    unsigned short int planes;         // Number of color planes
    unsigned short int bit_count;      // Bits per pixel
    unsigned int compression;          // Compression type
    unsigned int image_size;           // Image size in bytes
    int x_pixels_per_meter, y_pixels_per_meter;
    unsigned int colors_used;          // Number of colors used
    unsigned int colors_important;     // Number of important colors
} BMPInfoHeader;
#pragma pack(pop)

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s input.bmp output.raw\n", argv[0]);
        return 1;
    }

    char* input_file = argv[1];
    char* output_file = argv[2];

    FILE* file = fopen(input_file, "rb");
    if (!file) {
        printf("Failed to open the BMP file\n");
        return 1;
    }

    BMPHeader header;
    BMPInfoHeader info_header;

    // Read BMP header
    fread(&header, sizeof(BMPHeader), 1, file);

    // Check if it's a BMP file
    if (header.type != 0x4D42) {
        printf("Not a BMP file\n");
        fclose(file);
        return 1;
    }

    // Read BMP info header
    fread(&info_header, sizeof(BMPInfoHeader), 1, file);

    // Check if it's a supported BMP format (24-bit uncompressed)
    if (info_header.bit_count != 24 || info_header.compression != 0) {
        printf("Unsupported BMP format\n");
        fclose(file);
        return 1;
    }

    // Calculate the width and height
    int width = info_header.width;
    int height = info_header.height;

    // Allocate memory for pixel data
    unsigned char* pixel_data = (unsigned char*)malloc(info_header.image_size);

    // Read pixel data
    fread(pixel_data, 1, info_header.image_size, file);

    // Close the input file
    fclose(file);

    // Save the pixel data to the output file
    FILE* output = fopen(output_file, "wb");
    if (!output) {
        printf("Failed to open the output file\n");
        free(pixel_data);
        return 1;
    }

    // Write the pixel data to the output file
    fwrite(pixel_data, 1, info_header.image_size, output);

    // Close the output file
    fclose(output);

    // Free allocated memory
    free(pixel_data);

    printf("BMP file converted to raw BMP data and saved as %s\n", output_file);

    return 0;
}
