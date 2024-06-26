#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

/* A naive implementation of the algo at
https://en.wikipedia.org/wiki/Min-max_heap */

static void * xrealloc(void *b, size_t num, size_t siz);

typedef int T;
struct min_max_heap {
	T *data;
	size_t cap;
	size_t len;
	size_t level;  /* level in the tree of d[len] */
};

static void swap(T *a, T *b) { T t = *a; *a = *b; *b = t; }

static int
compare_to_grand_parent(T *d, size_t i, int min)
{
	/* Compare the value at i to its grandparent.
	** If min is true, return true if node i is less than gp.
	** If min is false, return true if node i is greater than gp.
	*/
	size_t k = (i+1)/4 - 1;
	return min ? (i > 2 && d[i] < d[k]) : (i > 6 && d[i] > d[k]);
}


/* Swap node i up the tree, skipping levels */
static void
push_up_2(T *d, size_t i, int min)
{
	while (compare_to_grand_parent(d, i, min)) {
		size_t k = (i+1)/4 - 1;
		swap(d + i, d + k);
		i = k;
	}
}


static void
push_up(T *d, size_t i, int min)
{
	if (i < 1) {
		return;
	}

	size_t p = (i-1)/2;  // parent;

	/* If needed, swap current with parent */
	if ((min && (d[i] > d[p])) || (!min && d[i] < d[p])) {
		swap(d + i, d + p);
		i = p;
		min = !min;
	}
	/* Now push up the tree within the current level category. */
	push_up_2(d, i, min);
}


static void
min_max_push(struct min_max_heap *h, T v)
{
	while (h->len >= h->cap) {
		h->data = xrealloc(h->data, h->cap += 512, sizeof *h->data);
	}
	h->data[h->len] = v;
	push_up(h->data, h->len, !(h->level % 2));

	// When incrementing the length, we are moving to a new level iff
	// the new length is 2^n - 1 for some n.
	h->len += 1;
	if ((h->len & (h->len + 1)) == 0) {
		h->level += 1;
	}
}

static int
cmp(T a, T b, const int min)
{
	return min ? a < b : b < a;
}

static void
push_down_cmp(struct min_max_heap *h, size_t i, const int min)
{
	T *d = h->data;
	size_t e = h->len;  /* end */

	size_t m;   /* index of the extrema (child or grandchild) */
	size_t lc;  /* index of left child */
	size_t rc;  /* index of right child */
	size_t llc; /* index of left-left grandchild */
	size_t lrc; /* index of left-right grandchild */
	size_t rlc; /* index of right-left grandchild */
	size_t rrc; /* index of right-right grandchild */

	while(
		lc  = 2 * i + 1,
		rc  = lc + 1,
		llc = 2 * lc + 1,
		lrc = llc + 1,
		rlc = 2 * rc + 1,
		rrc = rlc + 1,
		lc < e
	) {
		/* Verify the heap invariants on the grand children */
		assert(llc >= e || !cmp(d[llc], d[lc], !min));
		assert(lrc >= e || !cmp(d[lrc], d[lc], !min));
		assert(rlc >= e || !cmp(d[rlc], d[rc], !min));
		assert(rrc >= e || !cmp(d[rrc], d[rc], !min));

		T extrema;
		if (rrc < e) {
			extrema = d[m = llc];
			if (cmp(d[lrc], extrema, min)) extrema = d[m = lrc];
			if (cmp(d[rlc], extrema, min)) extrema = d[m = rlc];
			if (cmp(d[rrc], extrema, min)) extrema = d[m = rrc];
		} else {
			extrema = d[m = lc];
			assert(lc < e);
			if (rc < e && cmp(d[rc], extrema, min)) extrema = d[m = rc];
			if (llc < e && cmp(d[llc], extrema, min)) extrema = d[m = llc];
			if (lrc < e && cmp(d[lrc], extrema, min)) extrema = d[m = lrc];
			if (rlc < e && cmp(d[rlc], extrema, min)) extrema = d[m = rlc];
			assert (rrc >= e);
		}

		if (cmp(d[m], d[i], min)) {
			swap(d + m, d + i);
		}

		if (m == lc || m == rc) {
			return;
		}

		if (cmp(d[(m-1)/2], d[m], min)) {
			swap(d + m, d + (m-1)/2);
		}
		i = m;
	}
}


static void
decrement_len(struct min_max_heap *h)
{
	assert(h->len > 0);
	// if h->len is 2^n - 1, then we are decreasing level
	if ((h->len & (h->len + 1)) == 0) {
		h->level -= 1;
	}
	h->len -= 1;
}

static T
min_pop(struct min_max_heap *h)
{
	assert(h->len > 0);
	T *d = h->data;
	T rv = d[0];
	decrement_len(h);
	d[0] = d[h->len];
	push_down_cmp(h, 0, 1);
	return rv;
}


static T
max_pop(struct min_max_heap *h)
{
	assert(h->len > 0);
	T *d = h->data;
	decrement_len(h);
	if (h->len < 2) {
		return d[h->len];
	}
	size_t i = (d[1] > d[2]) ? 1 : 2;
	T rv = d[i];
	d[i] = d[h->len];
	push_down_cmp(h, i, 0);
	return rv;
}


int fail_count = 0;
int pass_count = 0;
#define validate(b) if( !(b) ) { fprintf(stderr, "FALSE: %s at line %d\n", \
	#b, __LINE__); fail_count += 1; } else { pass_count += 1; }

static void
test_1(void)
{
	struct min_max_heap h = {0};
	min_max_push(&h, 3);
	validate(3 == max_pop(&h));
	min_max_push(&h, 3);
	validate(3 == min_pop(&h));
	validate(0 == h.len);

	min_max_push(&h, 1);
	min_max_push(&h, 2);
	validate(2 == h.len);
	validate(1 == min_pop(&h));
	validate(2 == min_pop(&h));
	validate(0 == h.len);

	h.len = 0;
	h.level = 0;
	min_max_push(&h, 2);
	min_max_push(&h, 1);
	validate(2 == h.len);
	validate(1 == min_pop(&h));
	validate(2 == min_pop(&h));
	validate(0 == h.len);

	free(h.data);
}

static void
test_2(void)
{
	struct min_max_heap h = {0};
	for( int i = -5; i < 6; i += 1 ){
		min_max_push(&h, i);
	}
	for( int i = 5; i > 2; i -= 1 ){
		validate(i == max_pop(&h));
	}
	for( int i = -5; i < -1; i += 1 ){
		validate(i == min_pop(&h));
	}
	validate(2 == max_pop(&h));
	validate(-1 == min_pop(&h));
	validate(2 == h.len);

	for( int i = 0; i < 6; i += 1 ){
		min_max_push(&h, i);
	}
	/* Push a small value onto a max level */
	min_max_push(&h, -4);
	validate(-4 == min_pop(&h));
	for( int i = 6; i < 14; i += 1 ){
		min_max_push(&h, i);
	}
	min_max_push(&h, -5);
	validate(-5 == min_pop(&h));

	/* Pop max to cover push_down_cmp */
	validate(13 == max_pop(&h));

	free(h.data);
}


static void
test_push_up_min(void)
{
	T data[10] = { 5, 4, 1, 2, 3, 6, -7, 8 };
	struct min_max_heap h = {.data = data, .cap = 10, .len = 8};
	push_up_2(h.data, 3, 1);
	validate(data[0] == 2);
	validate(data[1] == 4);
	validate(data[2] == 1);
	validate(data[3] == 5);
	push_up_2(h.data, 6, 1);
	validate(data[0] == -7);
	validate(data[6] == 2);
}


static void
test_push_up_max(void)
{
	T data[10] = { 5,   4, 1,   2, 3, 6, -7,   8 };
	struct min_max_heap h = {.data = data, .cap = 10, .len = 8};
	push_up_2(h.data, 7, 0);
	validate(data[0] == 5);
	validate(data[1] == 8);
	validate(data[2] == 1);
	validate(data[3] == 2);
	validate(data[7] == 4);
}


static void
test_push_down_max_nollc(void)
{
	/* Test push down max when the item being pushed has
	no grand children */
	struct min_max_heap h = {0};
	int expect[] = { 0, 7, 6, 1, 3, 2, 5, 4 };

	/* First build a heap with 8 items */
	for (int i = 0; i < 8; i += 1) {
		min_max_push(&h, i);
	}
	for (int i = 0; i < 8; i += 1) {
		validate(h.data[i] == expect[i]);
	}
	validate(7 == max_pop(&h));

	/* Percolate the 3 down after pop to cover push_down_max */
	min_max_push(&h, 3);
	validate(6 == max_pop(&h));

	h.len = 0;
	h.level = 0;
	min_max_push(&h, 0);
	min_max_push(&h, 4);
	min_max_push(&h, 6);
	min_max_push(&h, 1);
	min_max_push(&h, 3);
	min_max_push(&h, 5);
	min_max_push(&h, 2);
	min_max_push(&h, 3);
	validate(6 == max_pop(&h));
	validate(5 == max_pop(&h));
	validate(4 == max_pop(&h));
	validate(3 == max_pop(&h));
	validate(3 == max_pop(&h));
	validate(2 == max_pop(&h));
	validate(1 == max_pop(&h));
	validate(0 == max_pop(&h));
	validate(h.len == 0);
	free(h.data);
}


static void
test_push_down_max_1(void)
{
	/* Get coverage of the linear max check of grand children */
	struct min_max_heap h = {0};
	min_max_push(&h, 0);
	min_max_push(&h, 4);
	min_max_push(&h, 6);
	min_max_push(&h, 1);
	min_max_push(&h, 3);
	min_max_push(&h, 2);
	min_max_push(&h, 5);
	/*
	*        0
	*   4         6
	* 1   3     2   5
	*/
	min_max_push(&h, 20);
	validate(h.data[1] == 20);
	validate(h.data[7] == 4);
	min_max_push(&h, 5);
	validate(h.data[8] == 5);
	min_max_push(&h, 6);
	min_max_push(&h, 7);
	min_max_push(&h, 1);  /* the 1 will be pushed down to the 7 */
	validate(20 == max_pop(&h));
	free(h.data);
}


static void
test_push_down_min_swap_rc(void)
{
	/* Get coverage of the swap with rc in push_down_min */
	struct min_max_heap h = {0};
	min_max_push(&h, 0);
	min_max_push(&h, 6);
	min_max_push(&h, 4);
	min_max_push(&h, 5);
	validate(0 == min_pop(&h));
	free(h.data);
}


static void
test_push_down_min_swap_rrc(void)
{
	/* Get coverage of the swap with rrc in push_down_min */
	struct min_max_heap h = {0};
	min_max_push(&h, 0);
	min_max_push(&h, 6);
	min_max_push(&h, 4);
	min_max_push(&h, 5);
	min_max_push(&h, 3);
	min_max_push(&h, 2);
	min_max_push(&h, 1);
	min_max_push(&h, 7);
	validate(0 == min_pop(&h));
	free(h.data);
}


static void
test_pairs(size_t siz, int which)
{
	assert(which == 0 || which == 1);

	struct min_max_heap h = {0};
	T (*poppers[])(struct min_max_heap *) = { min_pop, max_pop };

	/* Make a heap and validate by alternately popping max and min */
	for (int i = 0; i < siz; i++) {
		min_max_push(&h, i);
	}
	for (int i = 0; i < siz / 2; i++) {
		validate(poppers[which](&h) == (which ? siz - 1 - i : i));
		which = ! which;
		validate(poppers[which](&h) == (which ? siz - 1 - i : i));
	}
	if (siz % 2) {
		int i = siz / 2;
		validate(poppers[which](&h) == (which ? siz - 1 - i : i));
	}
	validate(0 == h.len);
	free(h.data);
}

static void
do_test_pairs(void)
{
	size_t sizes[] = {7, 32, 33, 63, 64, 65, 66, 1023, 1024};
	for (size_t *s = sizes; s < sizes + sizeof sizes / sizeof *sizes; s++) {
		for (int which = 0; which < 2; which += 1) {
			test_pairs(*s, which);
		}
	}
}


static void
test_constants(size_t limit)
{
	struct min_max_heap h = {0};
	for (size_t i=0; i < limit; i += 1) {
		min_max_push(&h, 3);
	}
	for (size_t i=0; i < limit; i += 1) {
		min_max_push(&h, 2);
		min_max_push(&h, 4);
	}
	for (size_t i=0; i < limit; i += 1) {
		validate(min_pop(&h) == 2);
		validate(max_pop(&h) == 4);
	}
	for (size_t i=0; i < limit/2; i += 1) {
		validate(max_pop(&h) == 3);
		validate(min_pop(&h) == 3);
	}
	free(h.data);
}


static void
start_timer(struct timeval *s)
{
	if (gettimeofday(s, NULL)) {
		perror("gettimeofday");
		exit(1);
	}
}


static void
report_time(const struct timeval *s)
{
	struct timeval end, diff;
	if( gettimeofday(&end, NULL)) {
		perror("gettimeofday");
		return;
	}

	timersub(&end, s, &diff);
	printf(
		"Elapsed time: %ld.%06lds\n",
		(long)diff.tv_sec,
		(long)diff.tv_usec
	);
	return;
}


int
main(int argc, char **argv)
{
	int count = argc > 1 ? strtol(argv[1], NULL, 10) : 1;
	struct timeval start;
	start_timer(&start);
	while (count--) {
	test_1();
	test_2();
	do_test_pairs();
	test_push_up_min();
	test_push_up_max();
	test_push_down_max_nollc();
	test_push_down_max_1();
	test_push_down_min_swap_rc();
	test_push_down_min_swap_rrc();
	test_constants(500);
	}
	report_time(&start);
	printf("%d tests passed, %d tests failed\n", pass_count, fail_count);
	return fail_count == 0 ? 0 : 1;
}


static void *
xrealloc(void *b, size_t num, size_t siz)
{
	b = realloc(b, num * siz);
	if( b == NULL ){
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	return b;
}
