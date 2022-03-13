

/* Sieve of Eratosthenes */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void * xcalloc(size_t s);

int
main(int argc, char **argv)
{
	long max = argc > 1 ? strtol(argv[1], NULL, 10) : 100;
	int c = 0;
	char *x = xcalloc(max);
	for( long p = 2; p < max; p += 1 ){
		if( x[p] ){
			continue;
		} else {
			printf("%s%8ld%s", c % 8 ? "\t" : "", p,
				c % 8 == 7 ? "\n" : "");
			c += 1;
		}
		for( long m = 2; m * p < max; m += 1 ){
			x[p * m] = 1;
		}
	}
	if( c % 8 ){
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
