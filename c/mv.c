/* Simple implementation of mv */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int
main(int argc, char **argv)
{
	const char *name = strchr(argv[0], '/');
	name = name == NULL ? argv[0] : name + 1;
	switch( argc ){
	case 1:
		printf("usage: %s src target\n", name);
		return EXIT_SUCCESS;
	case 2:
		fprintf(stderr, "Too few arguments\n");
		return EXIT_FAILURE;
	case 3:
		break;
	default:
		fprintf(stderr, "Unexpected arguments\n");
		return EXIT_FAILURE;
	}
	if( rename(argv[1], argv[2]) ){
		perror(argv[2]);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
