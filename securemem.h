#ifndef SECURE_MEM_H
#define SECURE_MEM_H

#include <unistd.h>
#include <sys/types.h>

typedef char * securemem_t; // More easily remember what is and isn't secure memory.

securemem_t secure_malloc(size_t size);

int secure_free(securemem_t ptr, size_t size);

securemem_t secure_calloc(size_t nmemb, size_t size);

securemem_t secure_realloc(securemem_t ptr, size_t new_size, size_t old_size);

#endif
