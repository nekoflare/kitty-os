#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input_filename> <output_filename> <variable_name>\n", argv[0]);
        return 1;
    }

    char* inputFileName = argv[1];
    char* outputFileName = argv[2];
    char* variableName = argv[3];

    // Open the input file for reading
    FILE* inputFile = fopen(inputFileName, "rb");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    // Open the output file for writing
    FILE* outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        fclose(inputFile);
        return 1;
    }

    // Write the beginning of the C source code
    fprintf(outputFile, "const char %s[] = {", variableName);

    // Read and write the file contents as a C char array
    int byte;
    int count = 0;
    while ((byte = fgetc(inputFile)) != EOF) {
        if (count % 12 == 0) {
            fprintf(outputFile, "\n    ");
        }
        fprintf(outputFile, "0x%02X, ", byte);
        count++;
    }

    // Write the end of the C source code
    fprintf(outputFile, "\n};\n");
    // Close the input and output files
    fclose(inputFile);
    fclose(outputFile);

    printf("File converted successfully. C code written to %s\n", outputFileName);

    return 0;
}
