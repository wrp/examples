#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

int fail = 0;  /* Count of failed tests */

struct user { char *name; int age; };
struct two_ints { int x, y; };

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


uint64_t
user_hash(const void *item, uint64_t seed0, uint64_t seed1)
{
	const struct user *user = item;
	return hashmap_sip(user->name, strlen(user->name), seed0, seed1);
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
	free(((struct user *)s)->name);
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
	struct user data[] = {
		{ .name="Dale", .age=44 },
		{ .name="Roger", .age=68 },
		{ .name="Jane", .age=47 },
	};
	assert( start <= sizeof data / sizeof *data );
	struct user *end = data + sizeof data / sizeof *data;
	for( struct user *t = data + start; t < end; t += 1 ){
		struct user d = { .name = strdup(t->name), .age = t->age };
		hashmap_set(map, &d);
		count -= 1;
	}
	while( count > 0 ){
		struct user d = { .name = strdup(base), .age = count-- };
		hashmap_set(map, &d);
		increment(base);
	}
}

static void
test_allocator_failures(void)
{
	struct user *user;
	struct hashmap *map;
	for( int i = 0; i < 2; i += 1 ){
		malloc_allow = i;
		map = hashmap_new_with_allocator(
			my_malloc, NULL, NULL,
			sizeof *user,
			0, 0, 0, user_hash, user_compare,
			NULL, NULL
		);
		expect( map == NULL );
	}

	/* With only two successful allocations, resize should fail */
	hashmap_set_allocator(my_malloc, my_free);
	malloc_allow = 2;
	map = hashmap_new_with_allocator(
		my_malloc, NULL, NULL, sizeof *user,
		0, 0, 0, user_hash, user_compare, NULL, NULL
	);
	load_data(map, 16, 0, NULL);
	expect( hashmap_oom(map) );
}

static void
test_probe(struct hashmap *m)
{
	hashmap_clear(m, false);

	struct user d = { .name = strdup("Barry"), .age = 5 };
	hashmap_set(m, &d);

	uint64_t h = user_hash(&d, 0, 0) & 0xf;

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

int
main(void)
{
	/*
	 * Create a new hash map. The second argument is the initial capacity.
	 * The third and fourth arguments are optional seeds that are passed to
	 * the hash function.  Fifth arg is hash function, 6th is comparison,
	 * 7th is free function, 8th is pointer passed to compar function.
	 */
	struct user *user;

	/*
	 * hashmap_set_allocator is deprecated.  Adding here only to get
	 * coverage before deleting entirely.  Note this is not true:
	 * currently, resize of a hashmap that has a custom allocator
	 * fails to use the allocator, so we must set it with
	 * hashmap_set_allocator.
	 */
	hashmap_set_allocator(malloc, free);
	struct hashmap *map = hashmap_new(
		sizeof *user + 1, /* Use wonky size to trigger code */
		0, 0, 0, user_hash, user_compare,
		free_el, NULL
	);

	load_data(map, 3, 0, NULL);

	user = hashmap_get(map, &(struct user){ .name="Jane" });
	expect( strcmp(user->name, "Jane") == 0 );
	expect( user->age == 47 );

	user = hashmap_get(map, &(struct user){ .name="Roger" });
	expect( strcmp(user->name, "Roger") == 0 );
	expect( user->age == 68 );

	user = hashmap_get(map, &(struct user){ .name="Dale" });
	expect( strcmp(user->name, "Dale") == 0 );
	expect( user->age == 44 );

	user = hashmap_get(map, &(struct user){ .name="Tom" });
	expect( user == NULL );

	/* terate over all users  */
	int i = 0;
	hashmap_scan(map, user_iter, &i);
	expect( i == 3 );

	/* Load enough data to trigger a resize */
	char name[32] = "Aale";
	load_data(map, 14, 3, name);

	i = 0;
	hashmap_scan(map, user_iter, &i);
	expect( i == 16 );
	user = hashmap_get(map, &(struct user){ .name="Dale" });
	expect( user != NULL && user->age == 11 );

	/* Insert "Abort" to terminate scan early */
	strcpy(name, "Abort");
	load_data(map, 1, 3, name);
	i = 0;
	hashmap_scan(map, user_iter, &i);
	expect( i != 16 );


	test_deletion(map);

	hashmap_clear(map, false);
	user = hashmap_get(map, &(struct user){ .name="Dale" });
	expect( user == NULL );

	test_probe(map);
	hashmap_free(map);

	test_allocator_failures();

	if( fail ){
		fprintf(stderr, "%d test%s failed\n", fail, &"s"[fail == 1]);
	}
	return fail;
}
