
/*
 * Read a polynomial and evaluate it
 * Toy problem from https://stackoverflow.com/questions/58831975/polynomial-evaluation-in-c
 * Sample format: 5x^1+23.5x^3-x^2
 *
 * Note that this fails to properly parse "5 + 5x + 5x^2"
 * Need to handle terms that have powers of 0 or 1.
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "xutil.h"

struct poly {
	int degree;
	double value;
	struct poly *next;
};

void * xmalloc(size_t s);

void
push(struct poly **head, const struct poly *this)
{
	(void)head;
	struct poly *new = xmalloc(sizeof *new);
	memcpy(new, this, sizeof *new);
	new->next = *head;
	*head = new;
}


void
parse_poly(struct poly **head, int argc, char *const*argv)
{
	/* Read the polynomial.*/
	struct poly this;
	int c;

	while( argc > 0 && (c = sscanf(argv[--argc], "%lf x^%d", &this.value, &this.degree)) == 2) {
		push(head, &this);
	}
}

double
eval(double x, const struct poly *p) {
	double result = 0;

	for(; p; p = p->next) {
		double term = 1;
		for(int i = 0; i < p->degree; i++) {
			term *= x;
		}
		result += term * p->value;
	}

	return result;
}
int
main(int argc, char **argv)
{
	struct poly *head = NULL;
	double val;

	parse_poly(&head, argc, argv);
	/* Read and evaluate arguments */

	for( struct poly *t = head; t; t=t->next ) {
		printf(" %+lg", t->value);
		switch(t->degree) {
		case 0: break;
		case 1: printf("x"); break;
		default: printf("x^%d", t->degree); break;
		}
	}
	putchar('\n');
	while( scanf("%lf ", &val) == 1 ) {
		printf("%lg: %lg\n", val, eval(val, head));
	}
	return 0;
}


void *
xmalloc(size_t s)
{
	void *rv = malloc(s);
	if(rv == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return rv;
}
