#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
	unsigned n = argc > 1 ? strtoul(argv[1], NULL, 10) : 10;
	for( unsigned i = 1; i <= n; i++ ){
		int f = EOF;
		if( i % 3 == 0 ){
			f = fputs("Fizz", stdout);
		}
		if( i % 5 == 0 ){
			f = fputs("Buzz", stdout);
		}
		if( f == EOF ){
			printf("%d", i);
		}
		putchar('\n');
	}
	return EXIT_SUCCESS;
}
