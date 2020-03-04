
/*
https://stackoverflow.com/questions/60515959/multiply-numbers-using-fork-and-a-pipe

For this assignment, we are required to multiply two numbers on
multiple processes. The first number gets multiplied by each digit
from the second number, multiplied by its position (1, 10th, 100th,
etc) and then sum all those numbers. Supposedly the sum will be
collected via the parent's pipe which is shared by all child
processes.
*/

#include <libgen.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


void
child(int a, char m, int *p)
{
	int rv = EXIT_SUCCESS;
	a *= m - '0';
	if(write(p[1], &a, sizeof a) == -1) {
		perror("write");
		rv = EXIT_FAILURE;
	}
	close(p[0]);
	close(p[1]);
	exit(rv);
}

int
main(int argc, char **argv)
{
	int a;
	int result = 0;
	char *b;
	int count;   /* Number of children to spawn */
	int fd[2];   /* Pipe to communicate back to the parent */
	if(argc != 3) {
		printf("usage: %s arg1 arg2\n", basename(argv[0]));
		return EXIT_SUCCESS;
	}
	a = strtol(argv[1], &b, 10);
	if( *b != '\0' ) {
		fprintf(stderr, "Invalid argument: %s\n", argv[1]);
		return EXIT_FAILURE;
	}
	b = argv[2];
	count = strlen(b);
	if(strspn(b,"0123456789") != count) {
		fprintf(stderr, "Invalid argument: %s\n", b);
		return EXIT_FAILURE;
	}
	if(pipe(fd) == -1) {
		perror("pipe");
		return EXIT_FAILURE;
	}
	for(int i = 0, mult = 1; i < count; i++) {
		int c;
		switch(fork()) {
		case -1:
			perror("fork");
			return EXIT_FAILURE;
		case 0:
			child(a, b[count - i - 1], fd );
			/* does not return */
		default:
			wait(NULL);
			if(read(fd[0], &c, sizeof c) == -1) {
				perror("read");
				return EXIT_FAILURE;
			}
			result += mult * c;
		}
		mult *= 10;
	}
	close(fd[0]);
	close(fd[1]);
	printf("%d * %s = %d\n", a, b, result);
}
