
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

int
maxArea(int *height, int heightSize)
{
	assert(heightSize > 1);
	int *left = height;
	int *right = height + heightSize - 1;
	int max = 0;

	do {
		int h = MIN(*left, *right);
		int v = h * (right - left);
		if (v > max) {
			max = v;
		}
		if (*left < *right) {
			do {
				left += 1;
			} while (*left <= h && left < right);
		} else {
			do {
				right -= 1;
			} while (*right <= h && left < right);
		}
	} while (right > left);
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
	t->heights = malloc(size * sizeof *t->heights);
	if( t->heights == NULL) {
		fputs("Out of memory\n", stderr);
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
