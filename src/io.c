#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static inline long po2_ceil(long initialValue);
void *readFile(const char *fileName, long *fileSize, bool po2);
int writeFile(const char *fileName, const void *buffer, size_t bufferSize);

static inline long po2_ceil(long initialValue) {
    int hammingWeight = 0;
    int lastBitTo1 = 0;

    for(unsigned long i = 0; i < sizeof(long); i++) {
        for(unsigned long j = 0; j < 8; j++) {
            unsigned long shift = i * 8 + j;
            int bit = (initialValue >> shift) & 1;

            if(bit) {
                lastBitTo1 = shift;
                hammingWeight++;
            }
        }
    }

    if(hammingWeight == 1) {
        return initialValue;
    } else {
        return 1 << (lastBitTo1 + 1);
    }
}

void *readFile(const char *fileName, long *fileSize, bool po2) {
    // Open the file as read-binary
    FILE *file = fopen(fileName, "rb");

    // If the file could not be opened, return an error
    if(!file) {
        fprintf(stderr, "readFile(): Failed to open %s.", fileName);
        return NULL;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long foundFileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Compare the file size
    if(*fileSize > 0 && foundFileSize > *fileSize) {
        fprintf(stderr, "readFile(): File is too large: %s.", fileName);
        return NULL;
    }

    long bufferSize = po2 ? po2_ceil(foundFileSize) : foundFileSize;

    // Allocate the buffer
    void *buffer = calloc(bufferSize, 1);

    if(!buffer) {
        fprintf(stderr, "readFile(): Failed to allocate buffer.\n");
        return NULL;
    }

    // Read the file
    size_t readBytes = fread(buffer, 1, foundFileSize, file);

    if(readBytes != (size_t)foundFileSize) {
        free(buffer);
        fprintf(stderr, "readFile(): Failed to read %ld bytes.\n", foundFileSize);
        return NULL;
    }

    *fileSize = bufferSize;

    return buffer;
}

int writeFile(const char *fileName, const void *buffer, size_t bufferSize) {
    // Open the file as read-binary
    FILE *file = fopen(fileName, "rb");

    // If the file could not be opened, return an error
    if(!file) {
        fprintf(stderr, "writeFile(): Failed to open %s.", fileName);
        return 1;
    }

    // Write the data to the file
    size_t writtenBytes = fwrite(buffer, 1, bufferSize, file);

    // Close the file
    fclose(file);

    // If the file could not be written, return an error
    if(writtenBytes != bufferSize) {
        fprintf(stderr, "writeFile(): Failed to write %ld bytes in %s.", bufferSize, fileName);
        return 1;
    }

    return 0;
}
