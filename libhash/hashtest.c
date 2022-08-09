#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

struct user {
	char *name;
	int age;
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
	const struct user *user = item;
	*(int *)udata += 1;
	return true;
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
bad_malloc(size_t size)
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


int
main(void)
{
	/* Create a new hash map. The second argument is the initial capacity.
	 * The third and fourth arguments are optional seeds that are passed to
	 * the hash function.  Fourth arg is hash function, 5th is comparison,
	 * 6th is free function, 7th is pointer passed to compar func
	 */
	int fail = 0;
	struct user *user;

	/* hashmap_set_allocator is deprecated.  Adding here only to get
	 * coverage before deleting entirely
	 */
	hashmap_set_allocator(malloc, free);
	struct hashmap *map = hashmap_new(
		sizeof *user + 1, /* Use wonky size to trigger code */
		0, 0, 0, user_hash, user_compare,
		NULL, NULL
	);

	// Here we'll load some users into the hash map. Each set operation
	// performs a copy of the data that is pointed to in the second argument.
	hashmap_set(map, &(struct user){ .name="Dale", .age=44 });
	hashmap_set(map, &(struct user){ .name="Roger", .age=68 });
	hashmap_set(map, &(struct user){ .name="Jane", .age=47 });

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
	hashmap_set(map, &(struct user){ .name="Aale", .age=44 });
	hashmap_set(map, &(struct user){ .name="Bale", .age=44 });
	hashmap_set(map, &(struct user){ .name="Cale", .age=44 });
	hashmap_set(map, &(struct user){ .name="Dale", .age=99 });
	hashmap_set(map, &(struct user){ .name="Eale", .age=44 });
	hashmap_set(map, &(struct user){ .name="Fale", .age=44 });
	hashmap_set(map, &(struct user){ .name="Gale", .age=44 });
	hashmap_set(map, &(struct user){ .name="Hale", .age=44 });
	hashmap_set(map, &(struct user){ .name="Iale", .age=44 });
	hashmap_set(map, &(struct user){ .name="Jale", .age=44 });
	hashmap_set(map, &(struct user){ .name="Kale", .age=44 });
	hashmap_set(map, &(struct user){ .name="Lale", .age=44 });
	hashmap_set(map, &(struct user){ .name="Male", .age=44 });
	hashmap_set(map, &(struct user){ .name="Nale", .age=44 });
	i = 0;
	hashmap_scan(map, user_iter, &i);
	expect( i == 16 );
	user = hashmap_get(map, &(struct user){ .name="Dale" });
	expect( user->age == 99 );

	hashmap_free(map);
	map = hashmap_new_with_allocator(
		bad_malloc, NULL, NULL,
		sizeof *user,
		0, 0, 0, user_hash, user_compare,
		NULL, NULL
	);
	expect( map == NULL );
	malloc_allow = 1;
	map = hashmap_new_with_allocator(
		bad_malloc, NULL, NULL,
		sizeof *user,
		0, 0, 0, user_hash, user_compare,
		NULL, NULL
	);
	expect( map == NULL );

	if( fail ){
		fprintf(stderr, "%d test%s failed\n", fail, &"s"[fail == 1]);
	}
	return fail;
}


