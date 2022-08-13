/*
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

struct bucket {
    uint64_t hash:48;
    uint64_t dib:16;
    char data[0];
};


// hashmap is an open addressed hash map using robinhood hashing.
struct hashmap {
    void *(*malloc)(size_t);
    void (*free)(void *);
    bool oom;
    struct hash_element el;
    size_t cap;
    struct hash_method hash;
    size_t bucketsz;
    size_t nbuckets;
    size_t count;
    size_t mask;
    size_t growat;
    void *buckets;
    void *spare;
    void *edata;
};

static struct bucket *bucket_at(struct hashmap *map, size_t index) {
    return (struct bucket*)(((char*)map->buckets)+(map->bucketsz*index));
}


static uint64_t
get_hash(struct hashmap *map, const void *key)
{
	uint64_t *seed = map->hash.seed;
	return map->hash.func(key, seed[0], seed[1]) & 0x0000ffffffffffff;
}

/*
 * return a new hash map using a custom allocator.
 * See hashmap_new for more details
 */
struct hashmap *
hashmap_new_with_allocator(
	void *(*_malloc)(size_t),
	void (*_free)(void*),
	const struct hash_element *el,
	const struct hash_method *hash,
	size_t cap
) {
	_malloc = _malloc ? _malloc : malloc;
	_free = _free ? _free : free;
	size_t ncap = 16;
	while( ncap < cap ){
		ncap *= 2;
	}
	cap = ncap;

	/*
	 * bucketsz is the size of a single bucket: enough
	 * to hold one element plus metadata in a leading struct bucket
	 */
	size_t bucketsz = sizeof(struct bucket) + el->size;

	/* Ensure that buckets are aligned well enough for uintptr_t */
	while( bucketsz & (sizeof(uintptr_t) - 1)) {
		bucketsz++;
	}
	/* hashmap + spare + edata */
	struct hashmap *map = _malloc(sizeof *map  + 2 * bucketsz);
	if( !map ){
		return NULL;
	}
	memset(map, 0, sizeof *map);
	map->el = *el;
	map->hash = *hash;
	map->bucketsz = bucketsz;
	map->spare = ((char*)map) + sizeof *map;
	map->edata = (char*)map->spare + bucketsz;
	map->cap = cap;
	map->nbuckets = cap;
	map->mask = map->nbuckets - 1;
	map->buckets = _malloc(map->bucketsz * map->nbuckets);
	if( !map->buckets ){
		_free(map);
		return NULL;
	}
	memset(map->buckets, 0, map->bucketsz * map->nbuckets);
	map->growat = map->nbuckets * 0.75;
	map->malloc = _malloc;
	map->free = _free;
	return map;
}


/* Create a new hash map. */
struct hashmap *
hashmap_new(
	const struct hash_element *el,
	const struct hash_method *hash, /* Hash method, with seeds */
	size_t cap                      /* Minimum initial capacity */
) {
	return hashmap_new_with_allocator(malloc, free, el, hash, cap);
}

static void
free_elements(struct hashmap *map)
{
	if( map->el.free ){
		for( size_t i = 0; i < map->nbuckets; i += 1 ){
			struct bucket *b = bucket_at(map, i);
			if( b->dib ){
				map->el.free(b->data);
			}
		}
	}
}


// hashmap_clear quickly clears the map.
// Every item is called with the element-freeing function given in hashmap_new,
// if present, to free any data referenced in the elements of the hashmap.
// When the update_cap is provided, the map's capacity will be updated to match
// the currently number of allocated buckets. This is an optimization to ensure
// that this operation does not perform any allocations.
void hashmap_clear(struct hashmap *map, bool update_cap) {
    map->count = 0;
    free_elements(map);
    if (update_cap) {
        map->cap = map->nbuckets;
    } else if (map->nbuckets != map->cap) {
        void *new_buckets = map->malloc(map->bucketsz*map->cap);
        if (new_buckets) {
            map->free(map->buckets);
            map->buckets = new_buckets;
        }
        map->nbuckets = map->cap;
    }
    memset(map->buckets, 0, map->bucketsz*map->nbuckets);
    map->mask = map->nbuckets-1;
    map->growat = map->nbuckets*0.75;
}


static void
swap(void *a, void *b, void *tmp, size_t s)
{
	memcpy(tmp, a, s);
	memcpy(a, b, s);
	memcpy(b, tmp, s);
}


static bool resize(struct hashmap *map, size_t new_cap) {
    struct hashmap *map2 = hashmap_new_with_allocator(
        map->malloc, map->free,
	&map->el, &map->hash, new_cap);
    if (!map2) {
        return false;
    }
    for (size_t i = 0; i < map->nbuckets; i++) {
        struct bucket *entry = bucket_at(map, i);
        if (!entry->dib) {
            continue;
        }
        entry->dib = 1;
        size_t j = entry->hash & map2->mask;
        for (;;) {
            struct bucket *bucket = bucket_at(map2, j);
            if (bucket->dib == 0) {
                memcpy(bucket, entry, map->bucketsz);
                break;
            }
            if (bucket->dib < entry->dib) {
		swap(bucket, entry, map2->spare, map->bucketsz);
            }
            j = (j + 1) & map2->mask;
            entry->dib += 1;
        }
	}
    map->free(map->buckets);
    map->buckets = map2->buckets;
    map->nbuckets = map2->nbuckets;
    map->mask = map2->mask;
    map->growat = map2->growat;
    map->free(map2);
    return true;
}


// hashmap_set inserts or replaces an item in the hash map. If an item is
// replaced then it is returned otherwise NULL is returned. This operation
// may allocate memory. If the system is unable to allocate additional
// memory then NULL is returned and hashmap_oom() returns true.
void *
hashmap_set(struct hashmap *map, void *item)
{
	assert( item != NULL );
	map->oom = false;
	if( map->count == map->growat ){
		if( !resize(map, 2 * map->nbuckets) ){
			map->oom = true;
			return NULL;
		}
	}

	int (*cmp)(const void *, const void *, void *) = map->el.compare;
	struct bucket *entry = map->edata;
	entry->hash = get_hash(map, item);
	entry->dib = 1;
	memcpy(entry->data, item, map->el.size);

	size_t i = entry->hash & map->mask;
	for (;;) {
		struct bucket *bucket = bucket_at(map, i);
		/* Empty bucket found; insert */
		if( bucket->dib == 0 ){
			memcpy(bucket, entry, map->bucketsz);
			map->count++;
			return NULL;
		}
		/* Entry found; replace */
		if( entry->hash == bucket->hash &&
			! cmp(entry->data, bucket->data, map->el.udata)
		){
			memcpy(map->spare, bucket->data, map->el.size);
			memcpy(bucket->data, entry->data, map->el.size);
			return map->spare;
		}
		/*
		 * Bucket full; find an empty hash slot
		 * This is the crux of Robinhood.  dib (I'm not sure
		 * what the original author intended this to mean, as
		 * dib seems to be what is commonly referred to as
		 * Probe Sequency Length (PSL)) is compared, and the
		 * item with the higher value gets inserted, while
		 * the entry with the lower value gets pushed back
		 * in the sequence.
		 */
		if( bucket->dib < entry->dib ){
			swap(bucket, entry, map->spare, map->bucketsz);
		}
		i = (i + 1) & map->mask;
		entry->dib += 1;
	}
}

// hashmap_get returns the item based on the provided key. If the item is not
// found then NULL is returned.
void *hashmap_get(struct hashmap *map, const void *key) {
    assert( key != NULL );
    uint64_t hash = get_hash(map, key);
	size_t i = hash & map->mask;
	for (;;) {
        struct bucket *bucket = bucket_at(map, i);
		if (!bucket->dib) {
			return NULL;
		}
		if (bucket->hash == hash &&
            map->el.compare(key, bucket->data, map->el.udata) == 0)
        {
            return bucket->data;
		}
		i = (i + 1) & map->mask;
	}
}

// hashmap_probe returns the item in the bucket at position or NULL if an item
// is not set for that bucket. The position is 'moduloed' by the number of
// buckets in the hashmap.
void *hashmap_probe(struct hashmap *map, uint64_t position) {
    size_t i = position & map->mask;
    struct bucket *bucket = bucket_at(map, i);
    if (!bucket->dib) {
		return NULL;
	}
    return bucket->data;
}


// hashmap_delete removes an item from the hash map and returns it. If the
// item is not found then NULL is returned.
void *hashmap_delete(struct hashmap *map, void *key) {
    assert( key != NULL );
    map->oom = false;
    uint64_t hash = get_hash(map, key);
	size_t i = hash & map->mask;
	for (;;) {
        struct bucket *bucket = bucket_at(map, i);
		if (!bucket->dib) {
			return NULL;
		}
		if (bucket->hash == hash &&
            map->el.compare(key, bucket->data, map->el.udata) == 0)
        {
            memcpy(map->spare, bucket->data, map->el.size);
            bucket->dib = 0;
            for (;;) {
                struct bucket *prev = bucket;
                i = (i + 1) & map->mask;
                bucket = bucket_at(map, i);
                if (bucket->dib <= 1) {
                    prev->dib = 0;
                    break;
                }
                memcpy(prev, bucket, map->bucketsz);
                prev->dib--;
            }
            map->count--;
			return map->spare;
		}
		i = (i + 1) & map->mask;
	}
}

/* Return the number of items in the hash map. */
size_t
hashmap_count(struct hashmap *map)
{
#ifndef NDEBUG
	size_t count = 0;
	for( size_t i = 0; i < map->nbuckets; i += 1 ){
		if( bucket_at(map, i)->dib ){
			count++;
		}
	}
	assert(count == map->count);
#endif
	return map->count;
}

// hashmap_free frees the hash map
// Every item is called with the element-freeing function given in hashmap_new,
// if present, to free any data referenced in the elements of the hashmap.
void hashmap_free(struct hashmap *map) {
    if (!map) return;
    free_elements(map);
    map->free(map->buckets);
    map->free(map);
}

// hashmap_oom returns true if the last hashmap_set() call failed due to the
// system being out of memory.
bool hashmap_oom(struct hashmap *map) {
    return map->oom;
}

// hashmap_scan iterates over all items in the hash map
// Param `iter` can return false to stop iteration early.
// Returns false if the iteration has been stopped early.
bool hashmap_scan(struct hashmap *map,
                  bool (*iter)(const void *item, void *udata), void *udata)
{
    for (size_t i = 0; i < map->nbuckets; i++) {
        struct bucket *bucket = bucket_at(map, i);
        if (bucket->dib) {
            if (!iter(bucket->data, udata)) {
                return false;
            }
        }
    }
    return true;
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
static uint64_t SIP64(const uint8_t *in, const size_t inlen,
                      uint64_t seed0, uint64_t seed1)
{
#define U8TO64_LE(p) \
    {  (((uint64_t)((p)[0])) | ((uint64_t)((p)[1]) << 8) | \
        ((uint64_t)((p)[2]) << 16) | ((uint64_t)((p)[3]) << 24) | \
        ((uint64_t)((p)[4]) << 32) | ((uint64_t)((p)[5]) << 40) | \
        ((uint64_t)((p)[6]) << 48) | ((uint64_t)((p)[7]) << 56)) }
#define U64TO8_LE(p, v) \
    { U32TO8_LE((p), (uint32_t)((v))); \
      U32TO8_LE((p) + 4, (uint32_t)((v) >> 32)); }
#define U32TO8_LE(p, v) \
    { (p)[0] = (uint8_t)((v)); \
      (p)[1] = (uint8_t)((v) >> 8); \
      (p)[2] = (uint8_t)((v) >> 16); \
      (p)[3] = (uint8_t)((v) >> 24); }
#define ROTL(x, b) (uint64_t)(((x) << (b)) | ((x) >> (64 - (b))))
#define SIPROUND \
    { v0 += v1; v1 = ROTL(v1, 13); \
      v1 ^= v0; v0 = ROTL(v0, 32); \
      v2 += v3; v3 = ROTL(v3, 16); \
      v3 ^= v2; \
      v0 += v3; v3 = ROTL(v3, 21); \
      v3 ^= v0; \
      v2 += v1; v1 = ROTL(v1, 17); \
      v1 ^= v2; v2 = ROTL(v2, 32); }
    uint64_t k0 = U8TO64_LE((uint8_t*)&seed0);
    uint64_t k1 = U8TO64_LE((uint8_t*)&seed1);
    uint64_t v3 = UINT64_C(0x7465646279746573) ^ k1;
    uint64_t v2 = UINT64_C(0x6c7967656e657261) ^ k0;
    uint64_t v1 = UINT64_C(0x646f72616e646f6d) ^ k1;
    uint64_t v0 = UINT64_C(0x736f6d6570736575) ^ k0;
    const uint8_t *end = in + inlen - (inlen % sizeof(uint64_t));
    for (; in != end; in += 8) {
        uint64_t m = U8TO64_LE(in);
        v3 ^= m;
        SIPROUND; SIPROUND;
        v0 ^= m;
    }
    const int left = inlen & 7;
    uint64_t b = ((uint64_t)inlen) << 56;
    switch (left) {
    case 7: b |= ((uint64_t)in[6]) << 48;
    case 6: b |= ((uint64_t)in[5]) << 40;
    case 5: b |= ((uint64_t)in[4]) << 32;
    case 4: b |= ((uint64_t)in[3]) << 24;
    case 3: b |= ((uint64_t)in[2]) << 16;
    case 2: b |= ((uint64_t)in[1]) << 8;
    case 1: b |= ((uint64_t)in[0]); break;
    case 0: break;
    }
    v3 ^= b;
    SIPROUND; SIPROUND;
    v0 ^= b;
    v2 ^= 0xff;
    SIPROUND; SIPROUND; SIPROUND; SIPROUND;
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
static void MM86128(const void *key, const int len, uint32_t seed, void *out) {
#define	ROTL32(x, r) ((x << r) | (x >> (32 - r)))
#define FMIX32(h) h^=h>>16; h*=0x85ebca6b; h^=h>>13; h*=0xc2b2ae35; h^=h>>16;
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
    const uint32_t * blocks = (const uint32_t *)(data + nblocks*16);
    for (int i = -nblocks; i; i++) {
        uint32_t k1 = blocks[i*4+0];
        uint32_t k2 = blocks[i*4+1];
        uint32_t k3 = blocks[i*4+2];
        uint32_t k4 = blocks[i*4+3];
        k1 *= c1; k1  = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
        h1 = ROTL32(h1,19); h1 += h2; h1 = h1*5+0x561ccd1b;
        k2 *= c2; k2  = ROTL32(k2,16); k2 *= c3; h2 ^= k2;
        h2 = ROTL32(h2,17); h2 += h3; h2 = h2*5+0x0bcaa747;
        k3 *= c3; k3  = ROTL32(k3,17); k3 *= c4; h3 ^= k3;
        h3 = ROTL32(h3,15); h3 += h4; h3 = h3*5+0x96cd1c35;
        k4 *= c4; k4  = ROTL32(k4,18); k4 *= c1; h4 ^= k4;
        h4 = ROTL32(h4,13); h4 += h1; h4 = h4*5+0x32ac3b17;
    }
    const uint8_t * tail = (const uint8_t*)(data + nblocks*16);
    uint32_t k1 = 0;
    uint32_t k2 = 0;
    uint32_t k3 = 0;
    uint32_t k4 = 0;
    switch(len & 15) {
    case 15: k4 ^= tail[14] << 16;
    case 14: k4 ^= tail[13] << 8;
    case 13: k4 ^= tail[12] << 0;
             k4 *= c4; k4  = ROTL32(k4,18); k4 *= c1; h4 ^= k4;
    case 12: k3 ^= tail[11] << 24;
    case 11: k3 ^= tail[10] << 16;
    case 10: k3 ^= tail[ 9] << 8;
    case  9: k3 ^= tail[ 8] << 0;
             k3 *= c3; k3  = ROTL32(k3,17); k3 *= c4; h3 ^= k3;
    case  8: k2 ^= tail[ 7] << 24;
    case  7: k2 ^= tail[ 6] << 16;
    case  6: k2 ^= tail[ 5] << 8;
    case  5: k2 ^= tail[ 4] << 0;
             k2 *= c2; k2  = ROTL32(k2,16); k2 *= c3; h2 ^= k2;
    case  4: k1 ^= tail[ 3] << 24;
    case  3: k1 ^= tail[ 2] << 16;
    case  2: k1 ^= tail[ 1] << 8;
    case  1: k1 ^= tail[ 0] << 0;
             k1 *= c1; k1  = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
    };
    h1 ^= len; h2 ^= len; h3 ^= len; h4 ^= len;
    h1 += h2; h1 += h3; h1 += h4;
    h2 += h1; h3 += h1; h4 += h1;
    FMIX32(h1); FMIX32(h2); FMIX32(h3); FMIX32(h4);
    h1 += h2; h1 += h3; h1 += h4;
    h2 += h1; h3 += h1; h4 += h1;
    ((uint32_t*)out)[0] = h1;
    ((uint32_t*)out)[1] = h2;
    ((uint32_t*)out)[2] = h3;
    ((uint32_t*)out)[3] = h4;
}

// hashmap_sip returns a hash value for `data` using SipHash-2-4.
uint64_t hashmap_sip(const void *data, size_t len,
                     uint64_t seed0, uint64_t seed1)
{
    return SIP64((uint8_t*)data, len, seed0, seed1);
}

// hashmap_murmur returns a hash value for `data` using Murmur3_86_128.
uint64_t hashmap_murmur(const void *data, size_t len,
                        uint64_t seed0, uint64_t seed1)
{
    char out[16];
    (void)seed1;
    MM86128(data, len, seed0, &out);
    return *(uint64_t*)out;
}
