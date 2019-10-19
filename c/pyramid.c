#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
 *      0
 *     101
 *    21012
 *   3210123
 *  432101234
 * 54321012345
 */

void write_line(int line, int size)
{
	char template[]="9876543210123456789";
	char *s = template + (9 - size);
	char *e = s + size + line;
	e[1] = '\0';
	memset(s, ' ', size - line);
	puts(s);
}

int
main(int argc, char **argv)
{
	int size = argc > 1 ? strtol(argv[1],NULL,10) : 5;
	if( size > 9 || size < 0) {
		fprintf(stderr, "Invalid size\n");
		return EXIT_FAILURE;
	}
	for(int line = 0; line <= size; line++ ) {
		write_line(line, size);
	}
}


