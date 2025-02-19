/* Trivial example using a binary heap to sort inputs */

/* keywords: priority queue, heap, min-heap */
#include <assert.h>
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

unsigned test_count;

#ifdef NDEBUG
#define Assert(x)
#else
#define Assert(x) do { assert(x); test_count += 1; } while(0)
#endif


/* Return 1 if the heap satsifies the invariant, 0 otherwise */
int
satisfies_invariant(struct heap *h)
{
	int *d = h->data;
	for( size_t i = 0; i < h->len / 2; i += 1 ){
		size_t left = 2 * i + 1;
		size_t right = 2 * i + 2;
		if( left < h->len && d[i] > d[left] ){
			return 0;
		}
		if( right < h->len && d[i] > d[right] ){
			return 0;
		}
	}
	return 1;
}


static void
up_heapify(struct heap *h)
{
	size_t i = h->len - 1;
	while( i > 0 && h->data[i] < h->data[(i - 1)/2] ){
		swap(h->data + i, h->data + (i - 1)/2);
		i = (i - 1)/2;
	}
	assert(satisfies_invariant(h));
}


/* Push a value onto the heap */
static void
push(struct heap *h, int v)
{
	if( h->len >= h->cap ){
		h->data = xrealloc(h->data, h->cap += 512, sizeof *h->data);
	}
	h->data[h->len++] = v;

	up_heapify(h);
}


static void
down_heapify(struct heap *h)
{
	int *d = h->data;
	size_t i = 0;
	size_t a;  /* index of left child */
	size_t b;  /* index of right child */
	size_t e = h->len; /* end */

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
	assert(satisfies_invariant(h));
}


/* Remove a value from the heap.  Behavior undefined if heap is empty. */
static int
pop(struct heap *h)
{
	int rv = h->data[0];
	h->data[0] = h->data[--h->len];
	down_heapify(h);
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
		Assert( g >= p );
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
test_invariant(void)
{
	int x[] = { 0, 1, 1, 2, 2, 2, 2, 3, 3, 3 };
	size_t siz = sizeof x / sizeof *x;
	struct heap h = { x, siz, siz };
	for( int i = 1; i < siz; i += 1 ){
		int orig = x[i];
		Assert( satisfies_invariant(&h) );
		x[i] -= 2;
		Assert( ! satisfies_invariant(&h) );;
		x[i] = orig;
	}
	Assert( satisfies_invariant(&h) );
	return 0;

}


void
test(void)
{
	basic_test();
	test_invariant();
}


int
main(int argc, char **argv)
{
	if( argc > 1 && ! strcmp(argv[1], "run") ){
		return interactive();
	} else {
		test();
		printf("%u tests passed\n", test_count);
		return 0;
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
