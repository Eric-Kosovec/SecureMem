#ifndef SECURE_UTIL_H
#define SECURE_UTIL_H

#include <sys/types.h>

void * secure_memset(void * s, int c, size_t n);

int secure_memcmp(const void * s1, const void * s2, size_t n);

#endif
