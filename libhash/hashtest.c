#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

int fail = 0;  /* Count of failed tests */

struct user { char *name; int age; char free; };
struct two_ints { int x, y; };
struct user testdata[] = {
	{ .name="", .age=5 },
	{ .name="Dale", .age=44 },
	{ .name="Roger", .age=68 },
	{ .name="Jane", .age=47 },
	{ .name=NULL, .age=0},
};

int
user_compare(const void *a, const void *b, void *udata)
{
	const struct user *ua = a;
	const struct user *ub = b;
	return strcmp(ua->name, ub->name);
}

bool
user_iter(const void *item, void *udata)
{
	const struct user *u = item;
	bool rv = strcmp(u->name, "Abort") == 0 ? false : true;
	if( rv ){
		*(int *)udata += 1;
	}
	return rv;
}


int
int_compare(const void *a, const void *b, void *udata)
{
	return *(int *)a - *(int *)b;
}


uint64_t
identity_hash(const void *int_pointer, uint64_t seed0, uint64_t seed1)
{
	return *(int *)int_pointer;
}

typedef uint64_t(*hash_func)(const void *item, uint64_t seed0, uint64_t seed1);

uint64_t
user_hash_sip(const void *item, uint64_t seed0, uint64_t seed1)
{
	const struct user *user = item;
	return hashmap_sip(user->name, strlen(user->name), seed0, seed1);
}

uint64_t
user_hash_murmur(const void *item, uint64_t seed0, uint64_t seed1)
{
	const struct user *user = item;
	return hashmap_murmur(user->name, strlen(user->name), seed0, seed1);
}

#define expect(x) if( !(x) ){ \
	fail += 1; \
	fprintf(stderr, "Test failure in %s at line %d: %s\n", \
		__FILE__, __LINE__, #x); \
	}

static int malloc_allow = 0;
static void *
my_malloc(size_t size)
{
	void *v;
	if( ! malloc_allow ){
		v = NULL;
	} else {
		malloc_allow -= 1;
		v = malloc(size);
	}
	return v;
}

static void
my_free(void *v)
{
	free(v);
	malloc_allow += (v != NULL);
}

static void
free_el(void *s)
{
	struct user *u = s;
	if( u->free ){
		free(u->name);
	}
}

/*
 * Roll a string through permutations (preserve case):
 * eg: abcd -> bbcd -> cbcd -> ... -> zbcd -> accd -> ... ->
 *     zzzy -> aaaz -> baaz -> ... -> yzzz -> zzzz -> aaaa
 */
static void
increment(char *t)
{
	while( *t == 'Z' || *t == 'z' ){
		*t = *t == 'Z' ? 'A' : 'a';
		t += 1;
	}
	if( *t ){
		*t += 1;
	}
}

/*
 * load some users into the hash map. Each set operation
 * performs a copy of the data that is pointed to in the second argument.
 */
static void
load_data(struct hashmap *map, unsigned count, unsigned start, char *base)
{
	char b[] = "Aaaa";
	if( base == NULL ){
		base = b;
	}
	assert( start <= sizeof testdata / sizeof *testdata );
	for( struct user *t = testdata + start; t->name && count; t += 1 ){
		hashmap_set(map, t);
		count -= 1;
	}
	while( count > 0 ){
		struct user d = {
			.name = strdup(base),
			.age = count--,
			.free = 1
		};
		hashmap_set(map, &d);
		increment(base);
	}
}

static void
test_allocator_failures(hash_func h)
{
	struct user *user;
	struct hashmap *map;
	for( int i = 0; i < 2; i += 1 ){
		malloc_allow = i;
		map = hashmap_new_with_allocator(
			my_malloc, NULL, NULL,
			sizeof *user,
			0, 0, 0, h, user_compare,
			NULL, NULL
		);
		expect( map == NULL );
	}

	/* With only two successful allocations, resize should fail */
	hashmap_set_allocator(my_malloc, my_free);
	malloc_allow = 2;
	map = hashmap_new_with_allocator(
		my_malloc, NULL, NULL, sizeof *user,
		0, 0, 0, h, user_compare, NULL, NULL
	);
	load_data(map, 16, 0, NULL);
	expect( hashmap_oom(map) );
	hashmap_free(map);
	hashmap_set_allocator(malloc, free);
}


uint64_t
mask(size_t cap)
{
	uint64_t ncap = 16;
	while( ncap < cap ){
		ncap *= 2;
	}
	return ncap - 1;
}


static void
test_probe(struct hashmap *m, hash_func hf, size_t cap)
{
	hashmap_clear(m, false);

	struct user d = { .name = strdup("Barry"), .age = 5 };
	hashmap_set(m, &d);

	uint64_t h = hf(&d, 0, 0) & mask(cap);

	struct user *a = hashmap_probe(m, h);
	struct user *u = hashmap_probe(m, !h);
	expect( a && !strcmp(a->name, "Barry") );
	expect( u == NULL );
}


static void
test_deletion(struct hashmap *m)
{
	struct user u = { .name = "kjljk", .age = 17 };
	struct user *up;

	hashmap_set(m, &u);
	up = hashmap_delete(m, &u);
	expect( up != NULL && up->age == 17 );

	/* Verify that get returns NULL */
	up = hashmap_get(m, &u);
	expect( up == NULL );

	/* delete a non-extant entry */
	up = hashmap_delete(m, &u);
	expect( up == NULL );

	/* Create a new map with trivial hash to test collisions */
	struct two_ints t = { 1, 2 };
	struct two_ints *tp;
	m = hashmap_new_with_allocator(
		malloc, realloc, free,
		sizeof t,
		0, 0, 0, identity_hash, int_compare,
		NULL, NULL
	);
	hashmap_set(m, &t);
	t.x = 17;
	t.y = 3;
	hashmap_set(m, &t);  /* collision */
	t.x = 1;
	tp = hashmap_get(m, &t);
	expect( tp && tp->y == 2);
	t.x = 17;
	tp = hashmap_get(m, &t);
	expect( tp && tp->y == 3);

	t.x = 1;
	/* Delete one of the entries */
	tp = hashmap_delete(m, &t);
	expect( tp && tp->y == 2);

	/* Add 16 entries to trigger a resize, then delete 15 to trigger
	 * a shrink.
	 */
	 for(int i = 0; i < 16; i++ ){
		t.x = i;
		t.y = i + 20;
		hashmap_set(m, &t);
	}
	for(int i = 1; i < 16; i++ ){
		t.x = i;
		tp = hashmap_delete(m, &t);
		expect( tp && tp->y == i + 20 );
	}
}

static void
test_hash(hash_func h, size_t cap)
{
	struct user *user;
	size_t testdata_size = sizeof testdata / sizeof *testdata - 1;

	/*
	 * hashmap_set_allocator is allegedly deprecated, but this is not true:
	 * currently, resize of a hashmap that has a custom allocator
	 * fails to use the allocator, so we must set it with
	 * hashmap_set_allocator.
	 */
	hashmap_set_allocator(malloc, free);

	/*
	 * Create a new hash map. The second argument is the initial capacity.
	 * The third and fourth arguments are optional seeds that are passed to
	 * the hash function.  Fifth arg is hash function, 6th is comparison,
	 * 7th is free function, 8th is pointer passed to compar function.
	 */
	struct hashmap *map = hashmap_new(
		sizeof *user + 1, /* Use wonky size to trigger code */
		cap, 0, 0, h, user_compare,
		free_el, NULL
	);

	/*
	 * Load all the test data and verify
	 */
	load_data(map, testdata_size, 0, NULL);
	for( struct user *u = testdata; u->name; u += 1 ){
		user = hashmap_get(map, u);
		expect( strcmp(user->name, u->name) == 0 );
		expect( user->age == u->age );
	}
	user = hashmap_get(map, &(struct user){ .name="Tom" });
	expect( user == NULL );
	/* terate over all users  */
	int i = 0;
	hashmap_scan(map, user_iter, &i);
	expect( i == testdata_size );

	/* Load enough data to trigger a resize */
	char name[32] = "Aale";
	unsigned load = 128; /* Amount of elements to load */
	load_data(map, load, testdata_size, name);

	i = 0;
	hashmap_scan(map, user_iter, &i);
	/* We expect "Dale" to be a duplicate, so subtract 1 */
	expect( i == testdata_size + load - 1);
	user = hashmap_get(map, &(struct user){ .name="Dale" });
	expect( user != NULL && user->age == load - 3 );

	/* Insert "Abort" to terminate scan early */
	strcpy(name, "Abort");
	load_data(map, 1, testdata_size, name);
	i = 0;
	hashmap_scan(map, user_iter, &i);
	expect( hashmap_count(map) == testdata_size + load );
	expect( i != testdata_size + load );

	/* Load a longer name to get coverage */
	strcpy(name, "Xxseven");
	load_data(map, 10, testdata_size, name);
	user = hashmap_get(map, &(struct user){ .name="Xxseven" });
	expect( user != NULL && user->age == 10 );

	test_deletion(map);
	test_probe(map, h, cap);

	load_data(map, load, 0, NULL);
	hashmap_clear(map, false);
	load_data(map, load, 0, NULL);
	hashmap_clear(map, true);
	user = hashmap_get(map, &(struct user){ .name="Dale" });
	expect( user == NULL );

	hashmap_clear(map, true);
	hashmap_free(map);

	test_allocator_failures(h);

	map = hashmap_new(
	        sizeof *user,
	        cap, 0, 0, h, user_compare,
	        free_el, NULL
	);
	char big_name[256];
	for( int i = 1; i < 255; i++ ){
	        big_name[i] = 'k';
	}
	big_name[0] = 'A';
	big_name[255] = '\0';
	load_data(map, 128, 0, big_name);
	user = hashmap_get(map, &(struct user){ .name="Dale" });
	expect( user && user->age == 44 );
	hashmap_free(map);
}

int
main(void)
{
	size_t sizes[] = { 0, 63, 237, 256 };
	size_t *end = sizes + sizeof sizes / sizeof *sizes;
	for( size_t *cap = sizes; cap < end; cap += 1 ){
		test_hash(user_hash_murmur, *cap);
		test_hash(user_hash_sip, *cap);
	}
	if( fail ){
		fprintf(stderr, "%d test%s failed\n", fail, &"s"[fail == 1]);
	}
	return fail;
}
