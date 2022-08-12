#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "hashmap.h"

static bool rand_alloc_fail = false;
static int rand_alloc_fail_odds = 3; // 1 in 3 chance malloc will fail.
static uintptr_t total_allocs = 0;
static uintptr_t total_mem = 0;

static void *xmalloc(size_t size) {
    if (rand_alloc_fail && rand()%rand_alloc_fail_odds == 0) {
        return NULL;
    }
    void *mem = malloc(sizeof(uintptr_t)+size);
	if( mem == NULL ){
		perror("malloc");
		exit(1);
	}
    *(uintptr_t*)mem = size;
    total_allocs++;
    total_mem += size;
    return (char*)mem+sizeof(uintptr_t);
}

static void xfree(void *ptr) {
    if (ptr) {
        total_mem -= *(uintptr_t*)((char*)ptr-sizeof(uintptr_t));
        free((char*)ptr-sizeof(uintptr_t));
        total_allocs--;
    }
}

static void shuffle(void *array, size_t numels, size_t elsize) {
    char tmp[elsize];
    char *arr = array;
    for (size_t i = 0; i < numels - 1; i++) {
        int j = i + rand() / (RAND_MAX / (numels - i) + 1);
        memcpy(tmp, arr + j * elsize, elsize);
        memcpy(arr + j * elsize, arr + i * elsize, elsize);
        memcpy(arr + i * elsize, tmp, elsize);
    }
}

static bool iter_ints(const void *item, void *udata) {
    int *vals = *(int**)udata;
    vals[*(int*)item] = 1;
    return true;
}

static int compare_ints_udata(const void *a, const void *b, void *udata) {
	(void)udata;
    return *(int*)a - *(int*)b;
}

static int compare_strs(const void *a, const void *b, void *udata) {
	(void)udata;
	return strcmp(*(char**)a, *(char**)b);
}

static uint64_t hash_int(const void *item, uint64_t seed0, uint64_t seed1) {
    return hashmap_murmur(item, sizeof(int), seed0, seed1);
}

static uint64_t hash_str(const void *item, uint64_t seed0, uint64_t seed1) {
    return hashmap_murmur(*(char**)item, strlen(*(char**)item), seed0, seed1);
}

static void free_str(void *item) {
    xfree(*(char**)item);
}

int
main(int argc, char **argv)
{
	unsigned N = argc > 1 ? strtoul(argv[1], NULL, 10) : 2000;
	unsigned seed = argc > 2 ? strtoul(argv[2], NULL, 10) : time(NULL);

    printf("seed=%d, count=%d, item_size=%zu\n", seed, N, sizeof(int));
    srand(seed);

    rand_alloc_fail = true;

    // test sip and murmur hashes
    assert(hashmap_sip("hello", 5, 1, 2) == 2957200328589801622);
    assert(hashmap_murmur("hello", 5, 1, 2) == 1682575153221130884);

    int *vals;
    while (!(vals = xmalloc(N * sizeof(int)))) {}
    for( unsigned i = 0; i < N; i++) {
        vals[i] = (int)i;
    }

    struct hashmap *map;
    struct hash_method hash = { hash_int, { seed, seed } };
    struct hash_element el = { sizeof *vals, compare_ints_udata };

    while( !(map = hashmap_new_with_allocator(xmalloc, xfree, &el, &hash, 0))){}
    shuffle(vals, N, sizeof(int));
    for (unsigned i = 0; i < (unsigned)N; i++) {
        assert(i == hashmap_count(map));
        int *v;
        assert(!hashmap_get(map, &vals[i]));
        assert(!hashmap_delete(map, &vals[i]));
        while (true) {
            assert(!hashmap_set(map, &vals[i]));
            if (!hashmap_oom(map)) {
                break;
            }
        }

        for (unsigned j = 0; j < i; j++) {
            v = hashmap_get(map, &vals[j]);
            assert(v && *v == vals[j]);
        }
        while (true) {
            v = hashmap_set(map, &vals[i]);
            if (!v) {
                assert(hashmap_oom(map));
                continue;
            } else {
                assert(!hashmap_oom(map));
                assert(v && *v == vals[i]);
                break;
            }
        }
        v = hashmap_get(map, &vals[i]);
        assert(v && *v == vals[i]);
        v = hashmap_delete(map, &vals[i]);
        assert(v && *v == vals[i]);
        assert(!hashmap_get(map, &vals[i]));
        assert(!hashmap_delete(map, &vals[i]));
        assert(!hashmap_set(map, &vals[i]));
        assert(i + 1 == hashmap_count(map));
    }

    int *vals2;
    while (!(vals2 = xmalloc(N * sizeof(int)))) {}
    memset(vals2, 0, N * sizeof(int));
    assert(hashmap_scan(map, iter_ints, &vals2));
    for (unsigned i = 0; i < N; i++) {
        assert(vals2[i] == 1);
    }
    xfree(vals2);

    shuffle(vals, N, sizeof(int));
    for (unsigned i = 0; i < N; i++) {
        int *v;
        v = hashmap_delete(map, &vals[i]);
        assert(v && *v == vals[i]);
        assert(!hashmap_get(map, &vals[i]));
        assert(N - i - 1 == hashmap_count(map));
        for (unsigned j = N-1; j > i; j--) {
            v = hashmap_get(map, &vals[j]);
            assert(v && *v == vals[j]);
        }
    }

    for (unsigned i = 0; i < N; i++) {
        while (true) {
            assert(!hashmap_set(map, &vals[i]));
            if (!hashmap_oom(map)) {
                break;
            }
        }
    }

    assert(hashmap_count(map) != 0);

    hashmap_clear(map, true);
    assert(hashmap_count(map) == 0);


    for (unsigned i = 0; i < N; i++) {
        while (true) {
            assert(!hashmap_set(map, &vals[i]));
            if (!hashmap_oom(map)) {
                break;
            }
        }
    }

    hashmap_clear(map, false);

    hashmap_free(map);

    xfree(vals);

    hash.func = hash_str;

    el.compare = compare_strs;
    el.free = free_str;
    el.size = sizeof(char *);
    while (!(map = hashmap_new_with_allocator( xmalloc, xfree, &el, &hash, 0)))
    	;

    for (unsigned i = 0; i < N; i++) {
        char *str;
        while (!(str = xmalloc(16)));
        sprintf(str, "s%i", i);
        while(!hashmap_set(map, &str));
    }

    hashmap_clear(map, false);
    assert(hashmap_count(map) == 0);

    for (unsigned i = 0; i < N; i++) {
        char *str;
        while (!(str = xmalloc(16)));
        sprintf(str, "s%i", i);
        while(!hashmap_set(map, &str));
    }

    hashmap_free(map);

    if (total_allocs != 0) {
        fprintf(stderr, "total_allocs: expected 0, got %lu\n", total_allocs);
        exit(1);
    }
    return 0;
}
