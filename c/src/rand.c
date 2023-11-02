#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Generate a random integer */

int
main(int argc, char **argv)
{
	int upper_bound = argc > 1 ? strtol(argv[1], NULL, 10) : 10;
	srand(time(NULL));

	printf("rand: %d\n", rand());
	printf( "non-uniform: %d\n", arc4random() % upper_bound);
	printf( "    uniform: %d\n", arc4random_uniform(upper_bound));

	return 0;
}
