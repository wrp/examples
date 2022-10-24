/*
 * Copyright:
 *   2020 Joshua J Baker
 *   2022 William R. Pursell
 * All rights reserved.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 */

#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct hashmap;

typedef uint64_t (*hash_function)(const void *, const void *);

struct hash_element {
	size_t size;  /* Size in bytes of each element */
	int (*compare)(const void *a, const void *b, void *udata);
	void (*free)(void *item);
	void *udata;  /* Data passed to comparison function */
};

struct hashmap *hashmap_new(
	const struct hash_element *,  /* Items stored in the map */
	hash_function,
	const void *seed,             /* passed as 2nd arg to hash_function */
	size_t                        /* Minimum initial capacity */
);

void hashmap_free(struct hashmap *);
void hashmap_clear(struct hashmap *);
size_t hashmap_count(struct hashmap *);
bool hashmap_oom(struct hashmap *);
void * hashmap_get(struct hashmap *, const void *key);
void * hashmap_set(struct hashmap *, void *item);
void * hashmap_delete(struct hashmap *, void *key);
void * hashmap_probe(struct hashmap *, uint64_t position);
int hashmap_scan(struct hashmap *, int (*)(const void *item, void *), void *);

uint64_t hashmap_sip(const void *, size_t, uint64_t, uint64_t);
uint64_t hashmap_murmur(const void *, size_t, uint64_t);

#endif
