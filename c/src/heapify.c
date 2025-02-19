/* Trivial example using a binary heap to sort inputs */

/* keywords: priority queue, heap, min-heap */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct heap {
	int *data;
	size_t cap;
	size_t len;
};
void *xrealloc(void *buf, size_t num, size_t siz);
static void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }


/* Push a value onto the heap */
static void
push(struct heap *h, int v)
{
	size_t i = h->len;
	if( h->len >= h->cap ){
		h->data = xrealloc(h->data, h->cap += 512, sizeof *h->data);
	}
	h->data[h->len++] = v;

	/* up heapify */
	while( i > 0 && h->data[i] < h->data[(i - 1)/2] ){
		swap(h->data + i, h->data + (i - 1)/2);
		i = (i - 1)/2;
	}
}


/* Remove a value from the heap.  Behavior undefined if heap is empty. */
static int
pop(struct heap *h)
{
	int *d = h->data;
	int rv = d[0];
	d[0] = d[--h->len];

	size_t i = 0;
	size_t a;  /* index of left child */
	size_t b;  /* index of right child */
	size_t e = h->len; /* end */

	/* down heapify */
	while(
		a = 2 * i + 1, b = a + 1,
		(a < e && d[i] > d[a]) || ( b < e && d[i] > d[b])
	){
		int t = b;
		if( b >= e || d[a] < d[b] ){
			t = a;
		}
		swap(d + i, d + t);
		i = t;
	}
	return rv;
}


/* Read an integer from stdin */
static int
get_value(int *v)
{
	char b[32] = "";
	char *end;
	int len;
	if( scanf("%31s%n", b, &len) == 1 && len < 31 ){
		*v = strtol(b, &end, 10);
		if( *end ){
			goto error;
		}
		return 1;
	}
	if( feof(stdin) ){
		return 0;
	}
error:
	fprintf(stderr, "Invalid input%s%s\n", b[0] ? " near: " : "", b);
	exit(1);
}


int
interactive(void)
{
	struct heap h = { 0 };
	int v;
	while( get_value(&v) == 1 ){
		push(&h, v);
	}
	while( h.len > 0 ){
		printf("%d, ", pop(&h));
	}
	putchar('\n');
	return 0;
}


void
validate_and_empty_heap(struct heap *h)
{
	int p = pop(h);
	while( h->len ){
		int g = pop(h);
		if( g < p ){
			fprintf(stderr, "Out of order %d < %d\n", g, p);
			exit(1);
		}
		p = g;
	}
}


int
basic_test(void)
{
	int test_cases[][10] = {
		{ 5, 4, 3, 2, 1 },
		{-3, 7, -6, 8, -1, 10},
		{ 1, 2, 5, 4, 3, 7, 6 },
	};
	struct heap h = { 0 };
	for( int i = 10; i > -3; i -= 2 ){
		push(&h, i);
	}
	validate_and_empty_heap(&h);
	for( int t = 0; t < sizeof test_cases / sizeof *test_cases; t += 1 ){
		int *tc = test_cases[t];
		for( ; tc < (int *)(test_cases + t + 1); tc += 1 ){
			push(&h, *tc);
		}
		validate_and_empty_heap(&h);
	}
	return 0;
}


int
test(void)
{
	int rv = 0;
	rv |= basic_test();
	return rv;
}


int
main(int argc, char **argv)
{
	if( argc > 1 && ! strcmp(argv[1], "run") ){
		return interactive();
	} else {
		return test();
	}
}

void *
xrealloc(void *b, size_t num, size_t siz)
{
	b = realloc(b, num * siz);
	if( b == NULL ){
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	return b;
}
