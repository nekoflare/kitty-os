#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


typedef struct RamFS_FileEntry
{
    char filename[64];
    unsigned long long file_length;
    unsigned long long file_offset;
} RamFS_FileEntry;

// Define the structure for the header
typedef struct RamFS_Header
{
    unsigned long long num_files;
    unsigned long long size_of_ramfs;
} RamFS_Header;

// Function to create an initramfs binary file
void createInitramfs(char* outputFileName, char* files[], int numFiles)
{
    // Calculate the size of the header and file entries
    size_t headerSize = sizeof(RamFS_Header);
    size_t fileEntrySize = sizeof(RamFS_FileEntry);
    size_t totalSize = headerSize + (fileEntrySize * numFiles);

    // Create the header
    RamFS_Header header;
    header.num_files = numFiles;
    header.size_of_ramfs = totalSize;

    // Create or open the output binary file
    int outputFd = open(outputFileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    if (outputFd == -1)
    {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    // Write the header to the output file
    ssize_t bytesWritten = write(outputFd, &header, headerSize);
    if (bytesWritten == -1)
    {
        perror("Error writing header to output file");
        close(outputFd);
        exit(EXIT_FAILURE);
    }

    // Write the files to the output file
    for (int i = 0; i < numFiles; i++)
    {
        // Open the input file
        int inputFd = open(files[i], O_RDONLY);
        if (inputFd == -1)
        {
            perror("Error opening input file");
            close(outputFd);
            exit(EXIT_FAILURE);
        }

        // Read the file content and write it to the output file
        char buffer[1024];
        ssize_t bytesRead;
        while ((bytesRead = read(inputFd, buffer, sizeof(buffer))) > 0)
        {
            ssize_t bytesWritten = write(outputFd, buffer, bytesRead);
            if (bytesWritten == -1)
            {
                perror("Error writing file content to output file");
                close(inputFd);
                close(outputFd);
                exit(EXIT_FAILURE);
            }
        }

        // Close the input file
        close(inputFd);

        // Create and write the file entry
        RamFS_FileEntry fileEntry;
        strncpy(fileEntry.filename, files[i], sizeof(fileEntry.filename));
        fileEntry.file_length = lseek(outputFd, 0, SEEK_CUR) - headerSize - fileEntrySize;
        fileEntry.file_offset = lseek(outputFd, 0, SEEK_CUR) - fileEntrySize;

        bytesWritten = write(outputFd, &fileEntry, fileEntrySize);
        if (bytesWritten == -1)
        {
            perror("Error writing file entry to output file");
            close(outputFd);
            exit(EXIT_FAILURE);
        }
    }

    // Close the output file
    close(outputFd);
}

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        fprintf(stderr, "Usage: %s <output_filename> <file1> <file2> ... <fileN>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* outputFileName = argv[1];
    char** files = &argv[2];
    int numFiles = argc - 2;

    createInitramfs(outputFileName, files, numFiles);

    printf("Initramfs created successfully: %s\n", outputFileName);

    return 0;
}
