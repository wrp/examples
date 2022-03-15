

/* Store words in input in a hashmap */
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "hashmap.h"

static uint64_t hash_str(const void *item, uint64_t s0, uint64_t s1) {
	return hashmap_murmur(*(char**)item, strlen(*(char**)item), s0, s1);
}
static int compare_strings(const void *a, const void *b, void *udata) {
	return strcmp(*(char**)a, *(char**)b);
}
static void free_str(void *item) {
	free(*(char**)item);
}

static bool print_string(const void *item, void *udata)
{
	printf("%s\n", *(char **)item);
	return true;
}
int
main(void)
{
	char buf[128];
	char *word;
	char *seed_env = getenv("SEED");
	int seed = seed_env ? strtol(seed_env, NULL, 0) : time(NULL);
	struct hashmap *map = hashmap_new(
		sizeof word, 0, seed, seed,
		hash_str, compare_strings, free_str, NULL
	);
	while( scanf("%127s", buf) == 1 ){
		word = strdup(buf);
		hashmap_set(map, &word);
	}


	hashmap_scan(map, &print_string, NULL);
}
