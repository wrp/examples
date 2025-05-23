#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct { int v, i; } T;
struct test_case {
	int target;
	int result[2];
	int *array;
	char line[256];
	size_t siz;
};

static void * xrealloc(void *b, size_t num, size_t siz);

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
	 * If min is true, return true if node i is less than gp.
	 * If min is false, return true if node i is greater than gp.
	 */
	size_t k = (i+1)/4 - 1;
	return min ? (i > 2 && d[i].v < d[k].v) : (i > 6 && d[i].v > d[k].v);
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

	size_t p = (i-1) / 2;

	/* If needed, swap current with parent */
	if ((min && (d[i].v > d[p].v)) || (!min && d[i].v < d[p].v)) {
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
		h->data = xrealloc(h->data, h->cap += 16384, sizeof *h->data);
	}
	h->data[h->len] = v;
	push_up(h->data, h->len, !(h->level % 2));

	/* When incrementing the length, we are moving to a new level iff
	 * the new length is 2^n - 1 for some n.
	 */
	h->len += 1;
	if ((h->len & (h->len + 1)) == 0) {
		h->level += 1;
	}
}

static int
cmp(T a, T b, const int min)
{
	return min ? a.v < b.v : b.v < a.v;
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
		T extrema = d[m = lc];
		if (rc >= e ) goto end;
		if (cmp(d[rc], extrema, min)) extrema = d[m = rc];
		if (llc >= e ) goto end;
		if (cmp(d[llc], extrema, min)) extrema = d[m = llc];
		if (lrc >= e ) goto end;
		if (cmp(d[lrc], extrema, min)) extrema = d[m = lrc];
		if (rlc >= e ) goto end;
		if (cmp(d[rlc], extrema, min)) extrema = d[m = rlc];
		if (rrc < e && cmp(d[rrc], extrema, min)) extrema = d[m = rrc];
		end:

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
	/* if h->len is 2^n - 1, then we are decreasing level */
	if ((h->len & (h->len + 1)) == 0) {
		h->level -= 1;
	}
	h->len -= 1;
}

static T
min_pop(struct min_max_heap *h)
{
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
	T *d = h->data;
	decrement_len(h);
	if (h->len < 2) {
		return d[h->len];
	}
	size_t i = (d[1].v > d[2].v) ? 1 : 2;
	T rv = d[i];
	d[i] = d[h->len];
	push_down_cmp(h, i, 0);
	return rv;
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


int *
twoSum(int *nums, int numsSize, int target, int *returnSize)
{
	struct min_max_heap h = {0};
	for(int i = 0; i < numsSize; i += 1 ){
		T a = { nums[i], i };
		min_max_push(&h, a);
	}
	*returnSize = 0;
	T a = min_pop(&h);
	T b = max_pop(&h);
	while(a.v + b.v != target) {
		if( a.v + b.v < target ){
			a = min_pop(&h);
		} else {
			b = max_pop(&h);
		}
	};
	int *rv = (int *)h.data;
	*returnSize = 2;
	if( a.i > b.i){
		int tmp = a.i;
		a.i = b.i;
		b.i = tmp;
	}
	rv[0] = a.i;
	rv[1] = b.i;
	return rv;
}

#if 1

void
push(struct test_case *t, int k)
{
	t->siz += 1;
	t->array = realloc(t->array, t->siz * sizeof *t->array);
	if( t->array == NULL ){
		perror("malloc");
		exit(1);
	}
	t->array[t->siz - 1] = k;
}


int
parse_test_case(const char *line, struct test_case *t)
{
	int k;
	const char *p;
	char *end;
	k = sscanf(line, "%d\t%d,%d\t[", &t->target, t->result, t->result + 1);
	if( k != 3 ){
		goto error;
	}
	t->array = NULL;
	t->siz = 0;
	p = strchr(line, '[');
	assert(p != NULL);
	p += 1;
	while( k = strtol(p, &end, 10), *end == ',' ){
		push(t, k);
		p = end + 1;
	}
	if( *end == ']' ){
		push(t, k);
		return 0;
	}
error:
	fprintf(stderr, "Invalid test case: %s", line);
	return 1;
}

int
get_test_case(struct test_case *t)
{
	if( fgets(t->line, sizeof t->line, stdin) == NULL ){
		return 0;
	}
	size_t len = strlen(t->line);
	if( len == 0 ){
		return get_test_case(t);
	}
	if( t->line[len - 1] != '\n' ){
		int c;
		fprintf(stderr, "Input too long.  Ignoring test!!\n");
		while( (c = getchar()) != EOF && c != '\n') {
			;
		}
		return get_test_case(t);
	}
	if( t->line[0] == 't' || t->line[0] == '#' ){
		return get_test_case(t);
	}
	if( parse_test_case(t->line, t)) {
		return get_test_case(t);
	}
	return 1;
}

int
main(int argc, char**argv )
{
	struct test_case t[1];
	int i = 0, *r;
	while( get_test_case(t) ){
		int k;
		i += 1;
		r = twoSum(t->array, t->siz, t->target, &k);
		if( r[0] != t->result[0] || r[1] != t->result[1] ){
			fprintf(stderr, "test %d failed: %s", i, t->line);
			return 1;
		}
	}
	fprintf(stderr, "%d tests passed\n", i);
	return 0;
}
#endif
