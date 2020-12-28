#ifndef __IO_H__
#define __IO_H__

#include <stdbool.h>
#include <stddef.h>

void *readFile(const char *fileName, long *fileSize, bool po2);
int writeFile(const char *fileName, const void *buffer, size_t bufferSize);

#endif
