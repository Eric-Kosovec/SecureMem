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
