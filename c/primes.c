/* Trivial demonstration of a linked list */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void * xmalloc(size_t s);
struct node { int val; struct node *next; };

/* Allocate a new struct node and push it to tail of the list. */
void
push(struct node *list, int val)
{
	while( list && list->next ) {
		list = list->next;
	}
	list->next = xmalloc(sizeof *list->next);
	list->next->val = val;
}

/* Extrememly inefficient method for detemining primality of a number */
int
is_prime(int x)
{
	int lim = ceil(sqrt(x)), i = 2;
	while( x % i && i <= lim ) {
		i += 1;
	}
	return i > lim;
}

/* Print the contents of the list. */
void
display(const struct node *list, char *delim, char *header)
{
	fputs(header, stdout);
	putchar(':');
	putchar('\t');
	for( const struct node *t = list->next; t; t = t->next ) {
		printf("%d%s", t->val, t->next ? delim : "");
	}
	putchar('\n');
}

int
main(void)
{
	struct node prime = {0, NULL};
	struct node composite = {0, NULL};
	struct node all = {0, NULL};
	int val;

	while( 1 == scanf("%d", &val ) ) {
		push(&all, val);
		push(is_prime(val) ? &prime : &composite, val);
	}

	display(&prime, ", ", "    primes");
	display(&composite, ", ", "non-primes");

	return EXIT_SUCCESS;
}

void *
xmalloc(size_t s)
{
	void *rv = malloc(s);
	if( rv == NULL ) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return rv;
}

