#ifndef __IO_H__
#define __IO_H__

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Reads the given file and returns a pointer to the buffer.
 * 
 * @param[in] fileName The path to the file to read.
 * @param[in,out] fileSize A buffer that will contain the size of the file. The
 *                         initial value in the buffer is the maximum allowed
 *                         file size.
 * @param[in] po2 A boolean value that indicates whether the size of the buffer
 *                should be rounded to the higher power of two number of bytes.
 * 
 * @returns A pointer to a buffer that contains the data read from the file.
 */
void *readFile(const char *fileName, long *fileSize, bool po2);

/**
 * @brief Writes the given buffer to the given file.
 * @details If the file already exists, it will be overwritten.
 * 
 * @param[in] fileName The path of the file to write.
 * @param[in] buffer The buffer to write to the file.
 * @param[in] bufferSize The number of bytes to write.
 * 
 * @returns An integer that indicates the result of the operation.
 * @retval 0 if the file was written correctly.
 * @retval Any other value if the operation failed.
 */
int writeFile(const char *fileName, const void *buffer, size_t bufferSize);

#endif
