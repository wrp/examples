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


int
main(int argc, char **argv)
{
	struct user *user;

	struct hash_element el = {
		.size = sizeof *user,
		.compare = user_compare
	};
	struct hashmap *map = hashmap_new(&el, user_hash_sip, 1024);

	struct user d = { "bob", 27 };
	hashmap_set(map, &d);
	d.name = "alice";
	d.age = 15;
	hashmap_set(map, &d);

	d.name = "bob";
	user = hashmap_get(map, &d);
	if( user ) {
		printf("name: %s, age: %d\n", user->name, user->age);
	} else {
		fprintf(stderr, "Not found\n");
	}
}
