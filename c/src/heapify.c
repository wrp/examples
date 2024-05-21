/* Trivial example using a binary heap to sort inputs */

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


/* Remove a value from the heap */
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

int
test(void)
{
	return 0;
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
