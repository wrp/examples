#include <stdio.h>
#include <inttypes.h>
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
	int i = 0;
	int c = '0' + line;
	for(; i < size - line; i++) {
		putchar(' ');
	}
	do putchar(c); while(c-- != '0');
	while(++c < '0' + line) putchar(c + 1);
	putchar('\n');
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


