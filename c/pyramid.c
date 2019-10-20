#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* Print a pyramid */

int
main(int argc, char **argv)
{
	int size = argc > 1 ? strtol(argv[1],NULL,10) : 5;
	if( size > 9 || size < 0) {
		fprintf(stderr, "Invalid size\n");
		return EXIT_FAILURE;
	}
	for(int line = 0; line <= size; line++) {
		char template[]="9876543210123456789";
		char *s = template + 9 - size;
		template[10 + line] = '\0';
		memset(s, ' ', size - line);
		puts(s);
	}
}
