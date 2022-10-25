/* Simple example showing usage */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hashmap.h"

struct user {
	char *name;
	int age;
	char free;
};

int
user_compare(const void *a, const void *b, void *udata)
{
	(void)udata;
	const struct user *ua = a;
	const struct user *ub = b;
	return strcmp(ua->name, ub->name);
}

uint64_t
user_hash_sip(const void *item, const void *seed)
{
	const struct user *user = item;
	return hashmap_sip(user->name, strlen(user->name), 0, 0);
}

struct user *
get(struct hashmap *h, char *name)
{
	return hashmap_get(h, &(struct user){ name });
}

struct user *
set(struct hashmap *h, char *name, int age)
{
	return hashmap_set(h, &(struct user){ name, age });
}

int
main(int argc, char **argv)
{
	struct user *user;

	struct hash_element el = {
		.size = sizeof *user,
		.compare = user_compare
	};
	struct hashmap *map = hashmap_new(&el, user_hash_sip, 1024);

	set(map, "bob", 27);
	set(map, "alice", 15);
	user = get(map, "bob");
	printf("name: %s, age: %d\n", user->name, user->age);
	user->age += 1;
	user = get(map, "bob");
	printf("name: %s, age: %d\n", user->name, user->age);

	get(map, "alice")->age += 1;
	user = get(map, "alice");
	printf("name: %s, age: %d\n", user->name, user->age);


	hashmap_free(map);
}
