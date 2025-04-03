/* Compute the squares of the first 20 natural numbers,
 * demonstrating fork and pipe */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define verify(x) if( !(x) ){ perror(#x); exit(EXIT_FAILURE); }
#define READ_SIDE(fd) ((fd)[0])
#define WRITE_SIDE(fd) ((fd)[1])

void
child(int start, const int *fd)
{
	verify(close(READ_SIDE(fd)) == 0);
	for( ; start < 21; start += 2 ){
		int product = start * start;
		verify(write(fd[1], &product, sizeof product) > 0);
	}
	verify(close(WRITE_SIDE(fd)) == 0);
	exit(0);
}

int
main(void)
{
	int odd[2], even[2];
	int *p[2] = { odd, even };
	int i, s;
	verify(pipe(odd) == 0);
	verify(pipe(even) == 0);

	for( i = 0; i < 2; i += 1 ){
		switch( fork() ){
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
		case 0:
			child(i + 1, p[i]);
		}
		verify(close(WRITE_SIDE(p[i])) == 0);
	}
	while( read(READ_SIDE(p[i++%2]), &s, sizeof s) > 0 ){
		printf("%d\n", s);
	}
	verify(close(READ_SIDE(odd)) == 0);
	verify(close(READ_SIDE(even)) == 0);
	return 0;
}
