
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
#include <stdio.h>
#include <unistd.h>


#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int
maxArea(int *height, int heightSize)
{
	int max = 0;
	int *a;
	for (int *a = height; a < height + heightSize - 1; a += 1) {
		for (int *b = a + 1; b < height + heightSize; b += 1) {
			int v = (b - a) * MIN(*a, *b);
			max = MAX(v, max);
		}
	}
	return max;
}

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
	t->heights = malloc(size * sizeof *t->heights);
	if (t->heights == NULL) {
		fprintf(stderr, "out of memory\n");
		exit(1);
	}
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
	struct test_case tests[2];
	struct test_case *end = tests + sizeof tests / sizeof *tests;
	init_test(tests + 0, 49, 9, 1, 8, 6, 2, 5, 4, 8, 3, 7);
	init_test(tests + 1, 1, 2, 1, 1);
	for (struct test_case *t = tests; t < end; t += 1) {
		int d = maxArea(t->heights, (int)t->siz);
		if (d != t->expected_value) {
			fprintf(stderr, "test %ld failed: d = %d, expect: %d\n",
				t - tests + 1, d, t->expected_value);
		}
	}
	return 0;
}
