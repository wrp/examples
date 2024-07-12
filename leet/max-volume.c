
/*
You are given an integer array height of length n. There are n
vertical lines drawn such that the two endpoints of the ith line
are (i, 0) and (i, height[i]).

Find two lines that together with the x-axis form a container, such
that the container contains the most water.

Return the maximum amount of water a container can store.

Notice that you may not slant the container.
*/

#include <stdarg.h>


#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>


#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

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

struct point { int x; int h; };
struct queue {
	struct point *data;
	size_t cap;
	size_t len;
};


static void swap(struct point *a, struct point *b) { struct point t = *a; *a = *b; *b = t; }

/* Remove a value from the queue.  Behavior undefined if queue is empty. */
static struct point
pop(struct queue *q)
{
	struct point *d = q->data;
	struct point rv = d[0];
	d[0] = d[--q->len];

	size_t i = 0;
	size_t a;  /* index of left child */
	size_t b;  /* index of right child */
	size_t e = q->len; /* end */

	/* down heapify */
	while(
		a = 2 * i + 1, b = a + 1,
		(a < e && d[i].h < d[a].h) || ( b < e && d[i].h < d[b].h)
	){
		int t = b;
		if( b >= e || d[a].h > d[b].h ){
			t = a;
		}
		swap(d + i, d + t);
		i = t;
	}
	return rv;
}


struct queue *
build_queue(int *height, int siz)
{
	struct queue *q = xrealloc(NULL, 1, sizeof *q);
	q->data = xrealloc(NULL, siz, sizeof *q->data);
	q->len = 0;
	q->cap = siz;

	for (int j = 0; j < siz; j += 1) {
		size_t i = q->len;
		q->data[q->len].x = i;
		q->data[q->len++].h = height[i];
		/* up heapify */
		while (i > 0 && q->data[i].h > q->data[(i - 1)/2].h) {
			swap(q->data + i, q->data + (i - 1)/2);
			i = (i - 1)/2;
		}
	}
	return q;
}


static int
volume(struct point a, struct point b)
{
	return abs(a.x - b.x) * MIN(a.h, b.h);
}


int
maxArea(int *height, int heightSize)
{
	int max = 0;
	int *a;

	if (heightSize < 2) {
		return 0;
	}
	struct queue *q = build_queue(height, heightSize);

	struct point pa = pop(q);  /* left boundary */
	struct point pb = pop(q);  /* right boundary */

	assert(pa.x != pb.x);
	if (pa.x > pb.x) {
		swap(&pa, &pb);
	}

	max = volume(pa, pb);

	while (q->len) {
		struct point pc = pop(q);
		assert(pa.x < pb.x);

		if (pc.x < pa.x) {
			int m = volume(pc, pb);
			if (m > max) {
				max = m;
			}
			pa = pc;
		} else if (pc.x > pb.x) {
			int m = volume(pc, pa);
			if (m > max) {
				max = m;
			}
			pb = pc;
		}
	}

	return max;
}


/*****************************/
struct test_case {
	int expected_value;
	int *heights;
	size_t siz;
};

void
init_test(struct test_case *t, int expect, size_t size, ...)
{
	t->expected_value = expect;
	t->siz = size;
	t->heights = xrealloc(NULL, size, sizeof *t->heights);
	va_list ap;
	int *h = t->heights;
	va_start(ap, size);
	while (size--) {
		*h++ = va_arg(ap, int);
	}
	va_end(ap);
}

int
main(int argc, char **argv)
{
	struct test_case tests[8];
	struct test_case *end = tests + sizeof tests / sizeof *tests;

	init_test(tests + 0, 49, 9, 1, 8, 6, 2, 5, 4, 8, 3, 7);
	init_test(tests + 1, 1, 2, 1, 1);
	init_test(tests + 2, 4, 3, 2, 100, 3);
	init_test(tests + 3, 50, 4, 2, 100, 50, 3);
	init_test(tests + 4, 50, 6, 2, 2, 2, 100, 50, 3);
	init_test(tests + 5, 52, 6, 2, 2, 26, 100, 50, 3);
	init_test(tests + 6, 78, 6, 2, 26, 26, 100, 50, 3);
	init_test(tests + 7, 25, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

	for (struct test_case *t = tests; t < end; t += 1) {
		int d = maxArea(t->heights, (int)t->siz);
		if (d != t->expected_value) {
			fprintf(stderr, "test %ld failed: d = %d, expect: %d\n",
				t - tests + 1, d, t->expected_value);
		}
	}
	return 0;
}
