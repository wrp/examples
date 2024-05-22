#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* A naive implementation of the algo at
https://en.wikipedia.org/wiki/Min-max_heap */

int fail_count = 0;
int pass_count = 0;
#define validate(b) if( !(b) ) { fprintf(stderr, "FALSE: %s at line %d\n", \
	#b, __LINE__); fail_count += 1; } else { pass_count += 1; }

static void * xrealloc(void *b, size_t num, size_t siz);

typedef int T;
struct min_max_heap {
	T *data;
	size_t cap;
	size_t len;
};


static void
init(struct min_max_heap *h)
{
	free(h->data);
	h->data = NULL;
	h->cap = h->len = 0;
}


static int
level(size_t i)
{
	/* Find level in the tree.  eg, highest set bit */
	int rv = 0;
	i += 1;
	while(i >>= 1) {
		rv += 1;
	}
	return rv;
}

static size_t parent(size_t i) { assert(i > 0); return (i-1)/2; }
static int is_min_level(size_t i) { return ! (level(i) % 2); }
static int is_max_level(size_t i) { return (level(i) % 2); }
static void swap(T *a, T *b) { T t = *a; *a = *b; *b = t; }


static void
push_up_max(struct min_max_heap *h, size_t i)
{
	assert(is_max_level(i));
	if( level(i) < 2 ){
		return;
	}
	T *d = h->data;
	if (d[i] > d[parent(parent(i))]) {
		swap(d + i, d + parent(parent(i)));
		push_up_max(h, parent(parent(i)));
	}
}


static void
push_up_min(struct min_max_heap *h, size_t i)
{
	/* Take the item at index i and percolate up min levels*/
	assert(is_min_level(i));
	if( level(i) < 2 ){
		assert(level(i) == 0);
		return;
	}
	T *d = h->data;
	if (d[i] < d[parent(parent(i))]) {
		swap(d + i, d + parent(parent(i)));
		push_up_min(h, parent(parent(i)));
	}
}


static void
push_up(struct min_max_heap *h, size_t i)
{
	T *d = h->data;

	if( i < 2 ){
		return;
	}

	if (is_min_level(i)) {
		if (d[i] > d[parent(i)]) {
			swap(d + i, d + parent(i));
			push_up_max(h, parent(i));
		} else {
			push_up_min(h, i);
		}
	} else {
		if (d[i] < d[parent(i)]) {
			swap(d + i, d + parent(i));
			push_up_min(h, parent(i));
		} else {
			push_up_max(h, i);
		}
	}
}


static void
min_max_push(struct min_max_heap *h, T v)
{
	size_t i = h->len;
	while( h->len >= h->cap ){
		h->data = xrealloc(h->data, h->cap += 512, sizeof *h->data);
	}
	T *d = h->data;
	d[h->len] = v;
	push_up(h, h->len);
	h->len += 1;

	return;
}


static void
push_down_max(struct min_max_heap *h, size_t i)
{
	assert(is_max_level(i));
	T *d = h->data;
	size_t lc;  /* index of left child */
	size_t llc; /* index of left-left grandchild */
	size_t lrc; /* index of left-right grandchild */
	size_t rc;  /* index of right child */
	size_t rlc;  /* index of right-left grandchild */
	size_t rrc;  /* index of right-right grandchild */
	size_t e = h->len; /* end */

	while(
		lc = 2 * i + 1,
		rc = lc + 1,
		llc = 2 * lc + 1,
		lrc = llc + 1,
		rlc = 2 * rc + 1,
		rrc = rlc + 1,
		(rrc < e && d[i] < d[rrc]) ||
		(rlc < e && d[i] < d[rlc]) ||
		(lrc < e && d[i] < d[lrc]) ||
		(llc < e && d[i] < d[llc]) ||
		(llc >= e && lc < e && d[i] < d[lc]) ||
		(rlc >= e && rc < e && d[i] < d[rc])
	) {
		size_t new_index = 0;
		if (llc >= e) {
			if( rc >= e || d[lc] > d[rc]) {
				new_index = lc;
			} else {
				new_index = rc;
			}
		} else {
			T max = d[new_index = llc];
			if (lrc < e && d[lrc] > max) {
				max = d[new_index = lrc];
			}
			if (rlc < e && d[rlc] > max) {
				max = d[new_index = rlc];
			}
			if (rrc < e && d[rrc] > max) {
				max = d[new_index = rrc];
			}
		}
		assert(new_index > 0);
		assert(new_index < e);
		assert(d[new_index] > d[i]);
		swap(d + new_index, d + i);
		i = new_index;
	}
}


static void
push_down_min(struct min_max_heap *h, size_t i)
{
	assert(is_min_level(i));
	T *d = h->data;
	size_t lc;  /* index of left child */
	size_t llc; /* index of left-left grandchild */
	size_t lrc; /* index of left-right grandchild */
	size_t rc;  /* index of right child */
	size_t rlc;  /* index of right-left grandchild */
	size_t rrc;  /* index of right-right grandchild */
	size_t e = h->len; /* end */

	while(
		lc = 2 * i + 1,
		rc = lc + 1,
		llc = 2 * lc + 1,
		lrc = llc + 1,
		rlc = 2 * rc + 1,
		rrc = rlc + 1,
		(rrc < e && d[i] > d[rrc]) ||
		(rlc < e && d[i] > d[rlc]) ||
		(lrc < e && d[i] > d[lrc]) ||
		(llc < e && d[i] > d[llc]) ||
		(llc >= e && lc < e && d[i] > d[lc]) ||
		(rlc >= e && rc < e && d[i] > d[rc])
	) {
		size_t new_index = 0;
		if (llc >= e) {
			if( rc >= e || d[lc] < d[rc]) {
				new_index = lc;
			} else {
				new_index = rc;
			}
		} else {
			T min = d[new_index = llc];
			if (lrc < e && d[lrc] < min) {
				min = d[new_index = lrc];
			}
			if (rlc < e && d[rlc] < min) {
				min = d[new_index = rlc];
			}
			if (rrc < e && d[rrc] < min) {
				min = d[new_index = rrc];
			}
		}
		assert(new_index > 0);
		assert(new_index < e);
		assert(d[new_index] < d[i]);
		swap(d + new_index, d + i);
		i = new_index;
	}
}


static void
push_down(struct min_max_heap *h, size_t i)
{
	if (is_min_level(i)) {
		push_down_min(h, i);
	} else {
		push_down_max(h, i);
	}
}


static T
min_pop(struct min_max_heap *h)
{
	assert(h->len > 0);
	T *d = h->data;
	T rv = d[0];
	d[0] = d[--h->len];
	push_down(h, 0);
	return rv;
}


static T
max_pop(struct min_max_heap *h)
{
	assert(h->len > 0);
	T *d = h->data;
	if(h->len < 3) {
		return d[--h->len];
	}
	size_t i = (d[1] > d[2]) ? 1 : 2;

	T rv = d[i];
	d[i] = d[--h->len];
	push_down(h, i);
	return rv;
}


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

	/* Pop max to cover push_down_max */
	validate(13 == max_pop(&h));

	free(h.data);
}


static void
test_level(void)
{
	validate(0 == level(0U));
	validate(1 == level(1U));
	validate(1 == level(2U));
	for (size_t i = 3; i < 7; i += 1) {
		validate(2 == level(i));
	}
	for (size_t e = 0; e < 10; e += 1) {
		size_t n = (1U << e);
		for (size_t i = n - 1; i < 2*n - 1; i += 1) {
			validate(e == level(i));
		}
	}
}


static void
test_push_up_min(void)
{
	T data[10] = { 5, 4, 1, 2, 3, 6, -7, 8 };
	struct min_max_heap h = {.data = data, .cap = 10, .len = 8};
	push_up_min(&h, 3);
	validate(data[0] == 2);
	validate(data[1] == 4);
	validate(data[2] == 1);
	validate(data[3] == 5);
	push_up_min(&h, 6);
	validate(data[0] == -7);
	validate(data[6] == 2);
}


static void
test_push_up_max(void)
{
	T data[10] = { 5,   4, 1,   2, 3, 6, -7,   8 };
	struct min_max_heap h = {.data = data, .cap = 10, .len = 8};
	push_up_max(&h, 7);
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

	init(&h);
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
}


int
main(int argc, char **argv)
{
	test_1();
	test_2();
	test_level();
	test_push_up_min();
	test_push_up_max();
	test_push_down_max_nollc();
	test_push_down_max_1();
	test_push_down_min_swap_rc();
	test_push_down_min_swap_rrc();

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
