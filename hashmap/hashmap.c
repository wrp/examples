/*
 * Open addressed hash map using robinhood hashing.
 *
 * Copyright:
 *   2020 Joshua J Baker
 *   2022 William R. Pursell
 * All rights reserved.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "hashmap.h"

#ifdef HASHMAP_TEST
#include <time.h>
#endif

#define HASH_BITS 48
#define PSL_BITS 16
#define HASH_MASK ( (1LU << HASH_BITS) - 1 )

static_assert(HASH_BITS + PSL_BITS == 64, "Invalid bucket sizes");

struct bucket {
	uint64_t hash:HASH_BITS;
	uint64_t psl:PSL_BITS;  /* Probe Sequence Length */
	char data[0];
};

struct hashmap {
	void *(*malloc)(size_t);
	void (*free)(void *);
	bool oom;
	struct hash_element el;
	struct hash_method hash; /* User defined hash method */
	size_t bucketsz;         /* Size of each bucket, padded for alignment */
	size_t nbuckets;         /* Total number of buckets in the table */
	size_t count;            /* Number of full buckets */
	size_t mask;             /* nbuckets - 1, used to bound index */
	size_t growat;           /* Threshold at which to grow the table */
	void *buckets;           /* The actual buckets */
	void *spare;             /* Work space used to return values */
	void *edata;             /* Work space used during insertion */
};

static struct bucket *
bucket_at(struct hashmap *map, size_t index)
{
	char *b = map->buckets;
	assert( index < map->nbuckets );
	return (struct bucket*)(b + map->bucketsz * index);
}


static uint64_t
get_hash(struct hashmap *map, const void *key)
{
	uint64_t *seed = map->hash.seed;
	return map->hash.func(key, seed[0], seed[1]) & HASH_MASK;
}

/*
 * Return a new hash map.
 */
struct hashmap *
hashmap_new_with_allocator(
	void *(*_malloc)(size_t),
	void (*_free)(void*),
	const struct hash_element *el,
	const struct hash_method *hash,
	size_t cap
)
{
	_malloc = _malloc ? _malloc : malloc;
	_free = _free ? _free : free;

	/* Ensure number of buckets is a power of 2 at least as big as cap */
	size_t nbuckets = 16;
	while( nbuckets < cap ){
		nbuckets *= 2;
	}

	/*
	 * bucketsz is the size of a single bucket (padded to align a
	 * uintptr_t) and is large enough to hold one element plus metadata
	 */
	size_t bucketsz = sizeof(struct bucket) + el->size;
	while( bucketsz & (sizeof(uintptr_t) - 1) ){
		bucketsz++;
	}

	/* hashmap + spare + edata */
	struct hashmap *map = _malloc(sizeof *map  + 2 * bucketsz);
	if( !map ){
		return NULL;
	}
	map->malloc = _malloc;
	map->free = _free;
	map->oom = false;;
	map->el = *el;
	map->hash = *hash;
	map->bucketsz = bucketsz;
	map->nbuckets = nbuckets;
	map->count = 0;
	map->mask = map->nbuckets - 1;
	map->growat = map->nbuckets * 0.75;
	map->buckets = _malloc(map->bucketsz * map->nbuckets);
	if( !map->buckets ){
		_free(map);
		return NULL;
	}
	memset(map->buckets, 0, map->bucketsz * map->nbuckets);
	map->spare = ((char*)map) + sizeof *map;
	map->edata = (char*)map->spare + bucketsz;
	return map;
}


/* Create a new hash map. */
struct hashmap *
hashmap_new(
	const struct hash_element *el,
	const struct hash_method *hash, /* Hash method, with seeds */
	size_t cap                      /* Minimum initial capacity */
)
{
	return hashmap_new_with_allocator(malloc, free, el, hash, cap);
}


/* Free every element in the map. */
static void
free_elements(struct hashmap *map)
{
	if( map->el.free ){
		for( size_t i = 0; i < map->nbuckets; i += 1 ){
			struct bucket *b = bucket_at(map, i);
			if( b->psl ){
				map->el.free(b->data);
			}
		}
	}
}


/*
 * Delete all entries in the map.
 */
void
hashmap_clear(struct hashmap *map)
{
	map->count = 0;
	free_elements(map);
	memset(map->buckets, 0, map->bucketsz * map->nbuckets);
}


/* Exchange data from bucket a to b, leaving a in m->spare */
static void
swap(const struct hashmap *m, void *a, void *b)
{
	size_t s = m->bucketsz;
	memcpy(m->spare, a, s);
	memcpy(a, b, s);
	memcpy(b, m->spare, s);
}


static bool
resize(struct hashmap *map, size_t new_cap)
{
	struct hashmap *map2 = hashmap_new_with_allocator(
		map->malloc,
		map->free,
		&map->el,
		&map->hash,
		new_cap
	);
	if( !map2 ){
		return false;
	}
	const typeof(map->mask) mask = map2->mask;

	/* Insert each element from map into map2.  We know that there
	 * are no duplicates, so we can avoid checking for that.  TODO:
	 * clean this up; there is much redundant code here that overlaps
	 * hashmap_set()
	 */
	for( size_t i = 0; i < map->nbuckets; i += 1 ){
		struct bucket *entry = bucket_at(map, i);
		if( !entry->psl ){
			continue;
		}
		entry->psl = 1;
		size_t j = entry->hash & mask;
		struct bucket *b = bucket_at(map2, j);
		while( 0 != (b = bucket_at(map2, j))->psl ){
			if( b->psl < entry->psl ){
				swap(map2, b, entry);
			}
			j = (j + 1) & mask;
			entry->psl += 1;
		}
                memcpy(b, entry, map->bucketsz);
#ifndef NDEBUG
		map2->count += 1;
#endif

	}
	assert( map2->count == map->count );
	assert( map2->malloc == map->malloc );
	assert( map2->free == map->free );
	assert( map2->bucketsz == map->bucketsz );
	assert( map2->mask == mask );

	map->free(map->buckets);
	map->buckets = map2->buckets;
	map->nbuckets = map2->nbuckets;
	map->mask = mask;
	map->growat = map2->growat;
	map->free(map2);

	return true;
}


static inline int
hash_match(const struct hashmap *m, const struct bucket *a,
	uint64_t h, const void *data)
{
	return a->hash == h && ! m->el.compare(a->data, data, m->el.udata);
}


/*
 * Insert or replace an item in the hash map.  Return NULL or the
 * replaced item.  This operation may allocate memory; if memory is
 * not available, NULL is returned and hashmap_oom() will return true.
 */
void *
hashmap_set(struct hashmap *map, void *item)
{
	assert( map != NULL );
	assert( item != NULL );
	map->oom = false;
	if( map->count == map->growat ){
		if( !resize(map, 2 * map->nbuckets) ){
			map->oom = true;
			return NULL;
		}
	}

	struct bucket *entry = NULL;
	uint64_t hash = get_hash(map, item);
	uint64_t psl = 1; /* probe sequence length */
	const typeof(map->mask) mask = map->mask;

	for( size_t i = hash & mask; ; i = (i + 1) & mask ){
		struct bucket *bucket = bucket_at(map, i);
		/* Empty bucket found; insert */
		if( bucket->psl == 0 ){
			bucket->psl = psl;
			bucket->hash = hash;
			memcpy(bucket->data, item, map->el.size);
			map->count++;
			return NULL;
		}
		/* Entry found; replace */
		if( hash_match(map, bucket, hash, item) ){
			assert( entry == NULL || entry->psl == bucket->psl );
			assert( entry == NULL || entry->hash ==  bucket->hash );
			memcpy(map->spare, bucket->data, map->el.size);
			memcpy(bucket->data, item, map->el.size);
			return map->spare;
		}
		/*
		 * Bucket full; find an empty hash slot
		 * This is the crux of Robinhood.
		 * Probe Sequency Length (PSL)) is compared, and the
		 * item with the higher value gets inserted, while
		 * the entry with the lower value gets pushed back
		 * in the sequence.
		 */
		if( bucket->psl < psl ){
			if( entry == NULL ){
				entry = map->edata;
				entry->hash = hash;
				memcpy(entry->data, item, map->el.size);
			}
			entry->psl = psl;
			swap(map, bucket, entry);
			item = entry->data;
			psl = entry->psl;
			hash = entry->hash;
		}
		psl += 1;
	}
}

/*
 * Return the item that matches the key, or NULL
 */
void *
hashmap_get(struct hashmap *map, const void *key)
{
	assert( key != NULL );
	uint64_t hash = get_hash(map, key);
	const typeof(map->mask) mask = map->mask;
	size_t i = hash & mask;
	struct bucket *bucket = bucket_at(map, i);
	while( bucket->psl ){
		if( hash_match(map, bucket, hash, key) ){
			return bucket->data;
		}
		/* TODO: implement a clean iterator function */
		bucket = bucket_at(map, i = (i + 1) & mask);
	}
	return NULL;
}

/*
 * Return the item in the bucket at position or NULL.
 */
void *
hashmap_probe(struct hashmap *map, uint64_t position)
{
	struct bucket *bucket = bucket_at(map, position & map->mask);
	return bucket->psl ? bucket->data : NULL;
}


/*
 * Remove an item from the hash map and return it.  If the
 * item is not found return NULL.
 */
void *
hashmap_delete(struct hashmap *map, void *key)
{
	assert( key != NULL );
	assert( map != NULL );
	map->oom = false;
	uint64_t hash = get_hash(map, key);
	const typeof(map->mask) mask = map->mask;
	size_t i = hash & mask;
	struct bucket *bucket = bucket_at(map, i);

	while( bucket->psl && ! hash_match(map, bucket, hash, key) ){
		bucket = bucket_at(map, i = (i + 1) & mask);
	}

	if( !bucket->psl ){
		return NULL;
	}

	memcpy(map->spare, bucket->data, map->el.size);

	/* Walk the  probe sequence and move buckets */
	for( ;; ){
		struct bucket *prev = bucket;
		i = (i + 1) & mask;
		bucket = bucket_at(map, i);

		if( bucket->psl < 2 ){
			/* This is the end of the cluster.  */
			prev->psl = 0;
			break;
		}
		bucket->psl -= 1;
		memcpy(prev, bucket, map->bucketsz);
		assert(prev->psl != 0);
	}
	map->count -= 1;
	return map->spare;
}

/* Return the number of items in the hash map. */
size_t
hashmap_count(struct hashmap *map)
{
#ifndef NDEBUG
	size_t count = 0;
	for( size_t i = 0; i < map->nbuckets; i += 1 ){
		if( bucket_at(map, i)->psl ){
			count++;
		}
	}
	assert(count == map->count);
#endif
	return map->count;
}

/*
 * Free the hash map, and every element.
 */
void
hashmap_free(struct hashmap *map)
{
	if( map != NULL ){
		free_elements(map);
		map->free(map->buckets);
		map->free(map);
	}
}

// hashmap_oom returns true if the last hashmap_set() call failed due to the
// system being out of memory.
bool hashmap_oom(struct hashmap *map) {
    return map->oom;
}

/*
 * Iterate over all items in the hash map.
 * iter() can return non-zero to stop iteration early
 * That value is returned.  (Zero return indicates that iter was called
 * on all items.)
 */
int
hashmap_scan(struct hashmap *map,
	int (*iter)(const void *item, void *udata), void *udata)
{
	/* TODO: For sparse maps, it doesn't make sense to iterate
	 * over all buckets, since they are mostly empty.  Maybe store
	 * first and last and just iterate over that interval, or count
	 * buckets that are full and terminate when we've done map->count
	 * buckets.
	 */
	int rv = 0;
	for( size_t i = 0; rv == 0 && i < map->nbuckets; i += 1 ){
		struct bucket *b = bucket_at(map, i);
		if( b->psl ){
			rv = iter(b->data, udata);
		}
	}
	return rv;
}

//-----------------------------------------------------------------------------
// SipHash reference C implementation
//
// Copyright (c) 2012-2016 Jean-Philippe Aumasson
// <jeanphilippe.aumasson@gmail.com>
// Copyright (c) 2012-2014 Daniel J. Bernstein <djb@cr.yp.to>
//
// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along
// with this software. If not, see
// <http://creativecommons.org/publicdomain/zero/1.0/>.
//
// default: SipHash-2-4
//-----------------------------------------------------------------------------
uint64_t
hashmap_sip(const void *data, size_t inlen, uint64_t seed0, uint64_t seed1)
{
	const uint8_t *in = data;

#define U8TO64_LE(p) ( \
	((uint64_t)((p)[0]) <<  0) | ((uint64_t)((p)[1]) <<  8) | \
	((uint64_t)((p)[2]) << 16) | ((uint64_t)((p)[3]) << 24) | \
	((uint64_t)((p)[4]) << 32) | ((uint64_t)((p)[5]) << 40) | \
	((uint64_t)((p)[6]) << 48) | ((uint64_t)((p)[7]) << 56) \
)
#define U32TO8_LE(p, v) \
	(p)[0] = (uint8_t)((v)); \
	(p)[1] = (uint8_t)((v) >> 8); \
	(p)[2] = (uint8_t)((v) >> 16); \
	(p)[3] = (uint8_t)((v) >> 24)
#define U64TO8_LE(p, v) \
	U32TO8_LE((p) + 0, (uint32_t)((v))); \
	U32TO8_LE((p) + 4, (uint32_t)((v) >> 32))
#define ROTL(x, b) (uint64_t)(((x) << (b)) | ((x) >> (64 - (b))))
#define SIPROUND \
	v0 += v1; \
	v1 = ROTL(v1, 13); \
	v1 ^= v0; \
	v0 = ROTL(v0, 32); \
	v2 += v3; \
	v3 = ROTL(v3, 16); \
	v3 ^= v2; \
	v0 += v3; \
	v3 = ROTL(v3, 21); \
	v3 ^= v0; \
	v2 += v1; \
	v1 = ROTL(v1, 17); \
	v1 ^= v2; \
	v2 = ROTL(v2, 32)

	uint64_t k0 = U8TO64_LE((uint8_t*)&seed0);
	fprintf(stderr, "k0 = %0llx  seed0 = %0llx  htonl = %0llx\n",
		k0, seed0, htonll(seed0));
	uint64_t k1 = U8TO64_LE((uint8_t*)&seed1);
	uint64_t v3 = UINT64_C(0x7465646279746573) ^ k1;
	uint64_t v2 = UINT64_C(0x6c7967656e657261) ^ k0;
	uint64_t v1 = UINT64_C(0x646f72616e646f6d) ^ k1;
	uint64_t v0 = UINT64_C(0x736f6d6570736575) ^ k0;
	const uint8_t *end = in + inlen - (inlen % sizeof(uint64_t));

	while( in != end ){
		uint64_t m = U8TO64_LE(in);
		v3 ^= m;
		SIPROUND;
		SIPROUND;
		v0 ^= m;
		in += 8;
	}
	uint64_t b = ((uint64_t)inlen) << 56;
	switch( inlen & 7 ){
	case 7: b |= ((uint64_t)in[6]) << 48;
	case 6: b |= ((uint64_t)in[5]) << 40;
	case 5: b |= ((uint64_t)in[4]) << 32;
	case 4: b |= ((uint64_t)in[3]) << 24;
	case 3: b |= ((uint64_t)in[2]) << 16;
	case 2: b |= ((uint64_t)in[1]) << 8;
	case 1: b |= ((uint64_t)in[0]);
	}
	v3 ^= b;
	SIPROUND;
	SIPROUND;
	v0 ^= b;
	v2 ^= 0xff;
	SIPROUND;
	SIPROUND;
	SIPROUND;
	SIPROUND;
	b = v0 ^ v1 ^ v2 ^ v3;
	uint64_t out = 0;
	U64TO8_LE((uint8_t*)&out, b);
	return out;
}

//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.
//
// Murmur3_86_128
//-----------------------------------------------------------------------------
uint64_t
hashmap_murmur(const void *key, size_t len, uint64_t seed, uint64_t seed1)
{
	char out[16];

	(void)seed1;

#define	ROTL32(x, r) (x = (((x << r) | (x >> (32 - r)))))
#define FMIX32(h) h^=h>>16; \
	h *= 0x85ebca6b; \
	h ^= h>>13; \
	h *= 0xc2b2ae35; \
	h ^= h>>16;

	const uint8_t * data = (const uint8_t*)key;
	const int nblocks = len / 16;
	uint32_t h1 = seed;
	uint32_t h2 = seed;
	uint32_t h3 = seed;
	uint32_t h4 = seed;
	uint32_t c1 = 0x239b961b;
	uint32_t c2 = 0xab0e9789;
	uint32_t c3 = 0x38b34ae5;
	uint32_t c4 = 0xa1e38b93;
	const uint32_t * blocks = (const uint32_t *)(data + nblocks * 16);
	for( int i = -nblocks; i; i += 1 ){
		uint32_t k1 = blocks[ i * 4 + 0 ];
		uint32_t k2 = blocks[ i * 4 + 1 ];
		uint32_t k3 = blocks[ i * 4 + 2 ];
		uint32_t k4 = blocks[ i * 4 + 3 ];

		k1 *= c1;
		ROTL32(k1, 15);
		k1 *= c2;

		h1 ^= k1;
		ROTL32(h1, 19);
		h1 += h2;
		h1 *= 5;
		h1 += 0x561ccd1b;

		k2 *= c2;
		ROTL32(k2, 16);
		k2 *= c3;

		h2 ^= k2;
		ROTL32(h2, 17);
		h2 += h3;
		h2 *= 5;
		h2 += 0x0bcaa747;

		k3 *= c3;
		ROTL32(k3, 17);
		k3 *= c4;

		h3 ^= k3;
		ROTL32(h3, 15);
		h3 += h4;
		h3 *= 5;
		h3 += 0x96cd1c35;

		k4 *= c4;
		ROTL32(k4, 18);
		k4 *= c1;

		h4 ^= k4;
		ROTL32(h4, 13);
		h4 += h1;
		h4 *= 5;
		h4 += 0x32ac3b17;
	}
	const uint8_t * tail = (const uint8_t*)(data + nblocks * 16);
	uint32_t k1 = 0;
	uint32_t k2 = 0;
	uint32_t k3 = 0;
	uint32_t k4 = 0;
	switch( len & 15 ){
	case 15: k4 ^= tail[14] << 16;
	case 14: k4 ^= tail[13] << 8;
	case 13: k4 ^= tail[12] << 0;
	/**/     k4 *= c4;
	/**/     ROTL32(k4,18);
	/**/     k4 *= c1;
	/**/     h4 ^= k4;
	case 12: k3 ^= tail[11] << 24;
	case 11: k3 ^= tail[10] << 16;
	case 10: k3 ^= tail[ 9] << 8;
	case  9: k3 ^= tail[ 8] << 0;
	/**/     k3 *= c3;
	/**/     ROTL32(k3,17);
	/**/     k3 *= c4;
	/**/     h3 ^= k3;
	case  8: k2 ^= tail[ 7] << 24;
	case  7: k2 ^= tail[ 6] << 16;
	case  6: k2 ^= tail[ 5] << 8;
	case  5: k2 ^= tail[ 4] << 0;
	/**/     k2 *= c2;
	/**/     ROTL32(k2,16);
	/**/     k2 *= c3;
	/**/     h2 ^= k2;
	case  4: k1 ^= tail[ 3] << 24;
	case  3: k1 ^= tail[ 2] << 16;
	case  2: k1 ^= tail[ 1] << 8;
	case  1: k1 ^= tail[ 0] << 0;
	/**/     k1 *= c1;
	/**/     ROTL32(k1,15);
	/**/     k1 *= c2;
	/**/     h1 ^= k1;
	};
	h1 ^= len;
	h2 ^= len;
	h3 ^= len;
	h4 ^= len;
	h1 += h2;
	h1 += h3;
	h1 += h4;
	h2 += h1;
	h3 += h1;
	h4 += h1;
	FMIX32(h1);
	FMIX32(h2);
	FMIX32(h3);
	FMIX32(h4);
	h1 += h2;
	h1 += h3;
	h1 += h4;
	h2 += h1;
	h3 += h1;
	h4 += h1;
	((uint32_t*)out)[0] = h1;
	((uint32_t*)out)[1] = h2;
	((uint32_t*)out)[2] = h3;
	((uint32_t*)out)[3] = h4;
	return *(uint64_t*)out;
}


#ifdef HASHMAP_TEST

clock_t begin;
int N;

static uint64_t
hash_int(const void *item, uint64_t seed0, uint64_t seed1)
{
	return hashmap_murmur(item, sizeof(int), seed0, seed1);
}

static int
compare_ints_udata(const void *a, const void *b, void *udata)
{
	(void)udata;
	return *(int*)a - *(int*)b;
}

static void
shuffle(void *array, size_t numels, size_t elsize)
{
	char tmp[elsize];
	char *arr = array;
	for( size_t i = 0; i < numels - 1; i++ ){
		int j = i + rand() / (RAND_MAX / (numels - i) + 1);
		memcpy(tmp, arr + j * elsize, elsize);
		memcpy(arr + j * elsize, arr + i * elsize, elsize);
		memcpy(arr + i * elsize, tmp, elsize);
	}
}

static clock_t
setup_benchmark(const char *name)
{
	printf("%-14s ", name);
	return begin = clock();
}


static void
show_bench_results(void)
{
	clock_t end = clock();
	double elapsed_secs = (double)(end - begin) / CLOCKS_PER_SEC;
	printf(
		"%d ops in %.3f secs, %.0f ns/op, %.0f op/sec",
		N,
		elapsed_secs,
		elapsed_secs / (double)N*1e9,
		(double)N / elapsed_secs
	);
	putchar('\n');
}


int
main(int argc, char **argv) /* benchmarks */
{
	N = argc > 1 ? strtol(argv[1], NULL, 10) : 5000000;
	int seed = argc > 2 ? strtol(argv[2], NULL, 10) : time(NULL);

	printf("seed = %d, count = %d, item_size = %zu\n", seed, N, sizeof N);
	srand(seed);

	int *vals = malloc(N * sizeof *vals);
	if( vals == NULL ){
		perror("malloc");
		return 1;
	}
	for( int i = 0; i < N; i += 1 ){
		vals[i] = i;
	}

	shuffle(vals, N, sizeof *vals);

	struct hashmap *map;
	shuffle(vals, N, sizeof *vals);

	struct hash_method hash = {hash_int, {seed, seed} };
	struct hash_element el = { sizeof *vals, compare_ints_udata, 0, 0 };
	map = hashmap_new_with_allocator(
		malloc, free, &el, &hash, 0
	);
	if( map == NULL ){
		perror("out of memory");
		exit(1);
	}

	setup_benchmark("set");
	for( int i = 0; i < N; i++ ){
		int *v = hashmap_set(map, &vals[i]);
		assert(!v);
	}
	show_bench_results();

	shuffle(vals, N, sizeof *vals);

	setup_benchmark("get");
	for( int i = 0; i < N; i++ ){
		int *v = hashmap_get(map, &vals[i]);
		assert(v && *v == vals[i]);
	}
	show_bench_results();

	shuffle(vals, N, sizeof *vals);
	setup_benchmark("delete");
	for( int i = 0; i < N; i++ ){
		int *v = hashmap_delete(map, &vals[i]);
		assert(v && *v == vals[i]);
	}
	show_bench_results();
	hashmap_free(map);

	map = hashmap_new_with_allocator( malloc, free, &el, &hash, N);
	if( map == NULL ){
		perror("out of memory");
		exit(1);
	}
	setup_benchmark("set (cap)");
	for( int i = 0; i < N; i++ ){
		int *v = hashmap_set(map, &vals[i]);
		assert(!v);
	}
	show_bench_results();

	shuffle(vals, N, sizeof *vals);
	setup_benchmark("get (cap)");
	for( int i = 0; i < N; i++ ){
		int *v = hashmap_get(map, &vals[i]);
		assert(v && *v == vals[i]);
	}
	show_bench_results();

	shuffle(vals, N, sizeof *vals);
	setup_benchmark("delete (cap)");
	for( int i = 0; i < N; i++ ){
		int *v = hashmap_delete(map, &vals[i]);
		assert(v && *v == vals[i]);
	}
	show_bench_results();

	hashmap_free(map);

	free(vals);
	return 0;
}
#endif
