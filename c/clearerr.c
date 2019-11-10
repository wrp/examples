#include <stdio.h>
#include <stdlib.h>

/*
 * Demonstrate the behavior of feof on a tty.  In particular,
 * running in bash and dash, closing the input stream is not final.
 * The stream from a tty is reopened and more data is sent.
 *
 * Is this a feature of the tty or the shell?
 */
int
main(void)
{
	int count = 0;
	while( count < 3 ) {
		int input = getchar();
		putchar(input);
		if( feof(stdin)) {
			puts("EOF");
			count += 1;
			clearerr(stdin);
		}
		if( ferror(stdin)) {
			perror("stdin");
			break;
		}
	}
	return EXIT_FAILURE;
}
