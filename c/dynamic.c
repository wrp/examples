#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "xutil.h"

/* Use getdelim to read comma-separated numbers */

struct data {
	size_t idx;
	size_t siz;
	double *val;
};

void Realloc(struct data *);
void push(struct data *, double);
int print(const struct data *);

int
main(int argc, char **argv)
{
	ssize_t rc;
	struct data V = {0};
	size_t bufsiz = 0;
	char *buf = NULL;
	FILE *ifp = argc > 1 ? xfopen(argv[1], "r") : stdin;

	Realloc(&V);

	while( (rc = getdelim( &buf, &bufsiz, ',', ifp)) > 0) {
		char *end, *prev=buf;
		push(&V, strtod(buf, &end));
		if( *end == '\n' && end[1] ) {
			push(&V, strtod(prev=end, &end));
		}
		if( end == prev || strcspn(end, ",\n") ) {
			fputs("invalid input\n", stderr);
			exit(EXIT_FAILURE);
		}
	}
	return print(&V);
}


void
Realloc(struct data *V)
{
	V->siz = V->siz ? V->siz * 2 : 1024;
	V->val = xrealloc(V->val, V->siz, sizeof *V->val, NULL);
	return;
}

void
push(struct data *V, double val)
{
	V->val[V->idx++] = val;
	if(V->idx == V->siz) {
		Realloc(V);
	}
}
int
print(const struct data *V)
{
	size_t i;
	for(i=0; i < V->idx; i++ ) {
		if(printf("%zu: %lg\n", i + 1, V->val[i]) < 0) {
			return EXIT_FAILURE; /* uncovered */
		}
	}
	return EXIT_SUCCESS;
}
