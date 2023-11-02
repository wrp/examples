

/* Sieve of Eratosthenes */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

void * xcalloc(size_t s);

int
main(int argc, char **argv)
{
	long max = argc > 1 ? strtol(argv[1], NULL, 10) : 100;
	int c = 1;
	char *x = xcalloc(max);
	long k = ceil(sqrt(max));
	for( long p = 2; p < max; p += 1 ){
		if( x[p] ){
			continue;
		}
		printf("%8ld%c", p, c ? '\t' : '\n');
		c = (c + 1) % 8;
		if( p < k ){
			for( long m = 2 * p; m < max; m += p ){
				x[m] = 1;
			}
		}
	}
	if( c != 1 ){
		putchar('\n');
	}
	return 0;
}

void *
xcalloc(size_t s)
{
	void *v = calloc(s, 1);
	if( v == NULL ){
		perror("calloc");
		exit(1);
	}
	return v;
}
