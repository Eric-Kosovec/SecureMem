#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

#include "securemem.h"
#include "secureutil.h"

// Some systems use _SC_PAGESIZE, while others use 
// PAGE_SIZE as a sysconf argument to retrieve the page size.
#ifdef _SC_PAGESIZE
#define PAGE_SIZE_ARG _SC_PAGESIZE
#else
#define PAGE_SIZE_ARG PAGE_SIZE
#endif

static int lock_mem(const securemem_t, size_t);
static int unlock_mem(const securemem_t, size_t);
static long get_page_size(void);

securemem_t secure_malloc(size_t size) {
	securemem_t ptr = NULL;
	
	if (size > 0 && ((ptr = malloc(size)) != NULL)) {
		// Lock pages containing the malloc'd memory so they 
		// do not get swapped to disk due to virtual memory.
		if (lock_mem(ptr, size)) {
			free(ptr);
			ptr = NULL;
		}
	}
	
	return ptr;
}

int secure_free(securemem_t ptr, size_t size) {
	int result = -1;
	
	if (size >= 0 && ptr) {
		result = 0;
		
		// Securely wipe the memory.
		secure_memset(ptr, 0, size);
		
		if (unlock_mem(ptr, size)) {
			result = -1;
		}
		
		// If an error occurs, still want to free the pointer,
		// as it was already zeroed out.
		free(ptr);
	}
	
	return result;
}

securemem_t secure_calloc(size_t nmemb, size_t size) {
	securemem_t ptr = NULL;
	
	if (nmemb >= 0 && size >= 0) {
		ptr = secure_malloc(nmemb * size);
	}
	
	return ptr;
}

securemem_t secure_realloc(securemem_t ptr, size_t new_size, size_t old_size) {
	securemem_t new_ptr = NULL;
	
	if (ptr && new_size >= 0 && old_size >= 0) {
		if ((new_ptr = secure_malloc(new_size)) == NULL) {
			secure_free(ptr, old_size);
			return NULL;
		}
		
		// Copy the data over.
		memcpy(new_ptr, ptr, (new_size >= old_size) ? old_size : new_size);
		
		// Free the old pointer.
		if (secure_free(ptr, old_size)) {
			secure_free(new_ptr, new_size);
			return NULL;
		}
	}
	
	return new_ptr;
}

static int lock_mem(const securemem_t ptr, size_t size) {
	int result = -1;
	
	long i;
	long page_size = get_page_size();
	
	if (ptr && size >= 0) {
		result = mlock(ptr, size);
		
		// Force Linux to reserve a physical page, 
		// as pages may be copy-on-write.
		for (i = 0; i < size; i += page_size) {
			((char *)ptr)[i] = '\0';
		}
	}
	
	return result;
}

static int unlock_mem(const securemem_t ptr, size_t size) {
	int result = -1;
	
	if (ptr && size >= 0) {
		result = munlock(ptr, size);
	}
	
	return result;
}

static long get_page_size() {
	return sysconf(PAGE_SIZE_ARG);
}
