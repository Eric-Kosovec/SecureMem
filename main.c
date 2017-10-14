#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "securemem.h"

int main(int argc, char * argv[]) {
	size_t bytes = 128;
	securemem_t pass = secure_malloc(bytes);
	
	if (!pass) {
		perror("Failed to acquire secure memory.");
		exit(EXIT_FAILURE);
	}
	
	memset(pass, 0, bytes);
	
	ssize_t bytes_read = 0;
	
	int i = 0;
	
	do {
		bytes_read = read(STDIN_FILENO, pass + i, sizeof(char));
		
		if (bytes_read <= 0) {
			secure_free(pass, bytes);
			perror("Failed to read bytes from stdin");
			exit(EXIT_FAILURE);
		}
		
		if (i + 1 == bytes) {
			pass = secure_realloc(pass, bytes << 1, bytes);
			
			if (!pass) {
				perror("Failed to realloc memory");
				exit(EXIT_FAILURE);
			}
			
			bytes <<= 1;
		}
		
		++i;
	} while (pass[i - 1] != '\n');
	
	pass[i - 1] = '\0';
	
	printf("Password is %s\n", (char *)pass);
	
	secure_free(pass, bytes);
	
	return EXIT_SUCCESS;
}