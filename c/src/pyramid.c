#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* Print a pyramid */

int
main(int argc, char **argv)
{
	unsigned size = argc > 1 ? strtoul(argv[1],NULL,10) : 5;
	if( size > 9 || size == 0 ){
		fprintf(stderr, "Invalid size\n");
		return EXIT_FAILURE;
	}
	for( unsigned line = 0; line <= size; line += 1 ){
		char template[]="9876543210123456789";
		template[10 + line] = '\0';
		char *end = template + 9;
		if( puts(memset(end - size, ' ', size - line)) == EOF ){
			perror("puts");
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
