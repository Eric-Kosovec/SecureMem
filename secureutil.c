#include "secureutil.h"

#include <stdlib.h>
#include <sys/types.h>

#define MEMORY_BARRIER() __asm__ __volatile__ ("" ::: "memory")

void * secure_memset(void * s, int c, size_t n) {
	char * t = (char *)s;
	
	int i;
	
	if (t && n > 0) {
		// Do not reorder memory access across the barrier, 
		// thus forcing the gcc compiler to include the call to this function 
		// and set each value even if the pointer given is never used after the call.
		MEMORY_BARRIER();
		for (i = 0; i < n; ++i) {
			t[i] = c;
		}
	}
	
	return s;
}

// Constant time comparison to avoid timing attacks.
int secure_memcmp(const void * s1, const void * s2, size_t n) {
	const char * t1 = (char *)s1;
	const char * t2 = (char *)s2;
	
	int result = 0;
	int i = 0;
	
	if (t1 == NULL || t2 == NULL || n < 0) {
		return 1;
	}
	
	for (i = 0; i < n; ++i) {		
		result |= t1[i] ^ t2[i];
	}
	
	return result;
}
