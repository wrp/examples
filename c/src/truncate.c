/* Remove the last line of a regular file */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
	int c = 0;
	off_t offset = -2;  /* If the last char is \n, skip it */
	const char *path = argc > 1 ? argv[1] : "test.txt";
	FILE *in = fopen(path, "r");
	if( in == NULL ){
		perror(path);
		return EXIT_FAILURE;
	}
	while( fseeko(in, offset, SEEK_END) == 0 && (c = fgetc(in)) != '\n' ){
		offset -= 1;
	}
	if( c != '\n' && fseek(in, 0, SEEK_SET) ){
		perror(path);
		return EXIT_FAILURE;
	}
	if( (offset = ftello(in)) == -1 || truncate(path, offset) != 0 ){
		perror(path);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
