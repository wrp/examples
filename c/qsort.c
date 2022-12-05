#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* demonstrate qsort
 *
 * declaration is:
 * void qsort(void *base, size_t nel, size_t width, int (*compar)(const void *, const void *));
 */


int
compare_string( const void *a, const void *b )
{
	return strcmp(*(char **)a,*(char **)b);
}

int
compare_char( const void *a, const void *b )
{
	return *(char *)a - *(char *)b;
}

int
main( int argc, char **argv )
{
	/* Sort each argument */
	puts("Arguments, individually sorted:");
	for( int i = 1; i < argc; i++ ) {
		char *copy;
		copy = strdup(argv[i]);
		qsort(copy, strlen(copy), 1, compare_char);
		putchar('\t');
		puts(copy);
		free(copy);
	}

	/* Sort the arguments */
	puts("Arguments, sorted:");
	qsort( argv + 1, argc - 1, sizeof *argv, compare_string );
	for( int i = 1; i < argc; i++ ) {
		putchar('\t');
		puts(argv[i]);
	}

	return 0;
}
