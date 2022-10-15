#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "hashmap.h"

int fail = 0;  /* Count of failed tests */
#define assert(x) if( !(x) ){ \
	fail += 1; \
	fprintf(stderr, "Test failure in %s at line %d: %s\n", \
		__FILE__, __LINE__, #x); \
	}

static bool rand_alloc_fail = false;
static int rand_alloc_fail_odds = 3; // 1 in 3 chance malloc will fail.
static uintptr_t total_allocs = 0;
static uintptr_t total_mem = 0;

static void *
xmalloc(size_t size)
{
	size_t *header;
	if( rand_alloc_fail && rand()%rand_alloc_fail_odds == 0 ){
		return NULL;
	}
	void *mem = header = malloc(sizeof header + size );
	if( mem == NULL ){
		perror("malloc");
		exit(1);
	}
	*header = size;
	total_allocs++;
	total_mem += size;
	return (char*)mem + sizeof header;
}

static void
xfree(void *ptr)
{
	if( ptr ){
		size_t *header = (void *)((char *)ptr - sizeof header);
		total_mem -= *header;
		free(header);
		total_allocs--;
	}
}

static void
shuffle(void *array, size_t numels, size_t elsize)
{
	char tmp[elsize];
	char *arr = array;
	for( size_t i = 0; i < numels - 1; i += 1 ){
		int j = i + rand() / (RAND_MAX / (numels - i) + 1);
		memcpy(tmp, arr + j * elsize, elsize);
		memcpy(arr + j * elsize, arr + i * elsize, elsize);
		memcpy(arr + i * elsize, tmp, elsize);
	}
}

static int
iter_ints(const void *item, void *udata)
{
	int *vals = *(int**)udata;
	vals[*(int*)item] = 1;
	return 0;
}

static int
compare_ints_udata(const void *a, const void *b, void *udata)
{
	(void)udata;
	return *(int*)a - *(int*)b;
}

static int
compare_strs(const void *a, const void *b, void *udata)
{
	(void)udata;
	return strcmp(*(char**)a, *(char**)b);
}

static uint64_t
hash_int(const void *item, uint64_t seed0, uint64_t seed1)
{
	return hashmap_murmur(item, sizeof(int), seed0, seed1);
}

static uint64_t
hash_str(const void *vitem, uint64_t seed0, uint64_t seed1)
{
	char *item = *(char **)vitem;
	return hashmap_murmur(item, strlen(item), seed0, seed1);
}

static void
free_str(void *item) {
	xfree(*(char**)item);
}

static void
test_exact_hashes(void)
{
    /* Some specific values */

	assert(hashmap_sip("hello", 5, 1, 2) == 2957200328589801622);
	assert(hashmap_murmur("hello", 5, 1, 2) == 1682575153221130884);

	for( int i = 0; i < 16; i += 1 ){
		uint64_t expect_sip[] = {
			0x8164b3f5d5f2a815,
			0x322130e9204fa827,
			0xa7c3dc82f3d3b138,
			0x725c97ff4b92866f,
			0x45dc70ae16a8ded5,
			0x2e98c26b5cbba9d5,
			0x9e74b6bf0d2a3ffc,
			0x3bcc5f49a24fa146,
			0xbd69d6c2d0d4963a,
			0x25072a3d8909dc87,
			0x6902037ffa456b82,
			0xa1138ae1d55d5ea8,
			0xd63ba52c7e065c3e,
			0x7b2f3531a3d29caf,
			0xe87af9a49a49bb13,
			0x290a1607eb437896
		};
		uint64_t expect_murmur[] = {
			0xdb91def72b2444a0,
			0xdb91def72b2444a0,
			0xdb91def72b2444a0,
			0xdb91def72b2444a0,
			0xdb91def72b2444a0,
			0xdb91def72b2444a0,
			0xdb91def72b2444a0,
			0xdb91def72b2444a0,
			0xdb25bad6f630f4d5,
			0x0b33f35188c7bcf2,
			0x672b90fb170eaed5,
			0x99fc51a9bf19dfa7,
			0xe00628d98b377b44,
			0xdcf199cb36f7ef9f,
			0x34f85a99aa572bad,
			0x1759b52feba4da84
		};
		uint64_t s = 0x1000000000000000 >> 4 * i;
		assert( hashmap_sip("hello", 5, s, 2) == expect_sip[i]);
		assert( hashmap_murmur("hello", 5, s, 2) == expect_murmur[i]);
	}

	for( int i = 0; i < 64; i += 1 ){
		uint64_t expect_sip[] = {
			0xcdd75081c4b09890,
			0x290a1607eb437896,
			0xc3234417737fac59,
			0x1f8b017da43b8b5e,
			0x4e1b26753c9f6e6f,
			0x1ba14d2dd45ba859,
			0x344f8047b85cbfcf,
			0x37f51adb9f7f4157,
			0xbc37e68f7535a7a0,
			0xa4ffe50b9a7b95af,
			0x8693b1ee579e0168,
			0x8c9c4996848b6d0a,
			0xb267994f6a2533ff,
			0xb99e0683f3babc0c,
			0x733d90f26bb10854,
			0x177a98eaf7dcd11d,
			0xf7207f15e487c71b,
			0x4dc46ad05454e929,
			0x702699880f1b7a94,
			0x64013c136ed649d1,
			0x4c476287ecc453ab,
			0x8b10d5e9d432baa2,
			0x2d614d0fc2f12fe9,
			0xb0b28abdf7345e38,
			0x1fca97d22010bbce,
			0x91de15642ee6ff72,
			0x52faf4743575e735,
			0x9d23dea622a05efd,
			0x88c2715a4e0edf1f,
			0x3a6d4cde98c816f0,
			0x64bcb59fc4d4d582,
			0x6e4081dab8374af6,
			0xcdd75081c4b09890,
			0x290a1607eb437896,
			0xc3234417737fac59,
			0x1f8b017da43b8b5e,
			0x4e1b26753c9f6e6f,
			0x1ba14d2dd45ba859,
			0x344f8047b85cbfcf,
			0x37f51adb9f7f4157,
			0xbc37e68f7535a7a0,
			0xa4ffe50b9a7b95af,
			0x8693b1ee579e0168,
			0x8c9c4996848b6d0a,
			0xb267994f6a2533ff,
			0xb99e0683f3babc0c,
			0x733d90f26bb10854,
			0x177a98eaf7dcd11d,
			0xf7207f15e487c71b,
			0x4dc46ad05454e929,
			0x702699880f1b7a94,
			0x64013c136ed649d1,
			0x4c476287ecc453ab,
			0x8b10d5e9d432baa2,
			0x2d614d0fc2f12fe9,
			0xb0b28abdf7345e38,
			0x1fca97d22010bbce,
			0x91de15642ee6ff72,
			0x52faf4743575e735,
			0x9d23dea622a05efd,
			0x88c2715a4e0edf1f,
			0x3a6d4cde98c816f0,
			0x64bcb59fc4d4d582,
			0x6e4081dab8374af6,
		};

		uint64_t s = 1 << i;
		const char *msg = "hello";
		assert( hashmap_sip(msg, 5, 1, s) == expect_sip[i]);
		assert( hashmap_murmur(msg, 5, 1, s) == 0x1759b52feba4da84 );
	}
}

int
main(int argc, char **argv)
{
	unsigned N = argc > 1 ? strtoul(argv[1], NULL, 10) : 2000;
	unsigned seed = argc > 2 ? strtoul(argv[2], NULL, 10) : time(NULL);

	printf("seed=%d, count=%d, item_size=%zu\n", seed, N, sizeof(int));
	srand(seed);

	rand_alloc_fail = true;

	test_exact_hashes();

	int *vals;
	do vals = xmalloc(N * sizeof *vals);
	while( vals == NULL );

	for( unsigned i = 0; i < N; i += 1 ){
		vals[i] = (int)i;
	}

	struct hashmap *map;
	struct hash_method hash = { hash_int, { seed, seed } };
	struct hash_element el = { sizeof *vals, compare_ints_udata };

	do map = hashmap_new_with_allocator(xmalloc, xfree, &el, &hash, 0);
	while( map == NULL );

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
    assert(0 == hashmap_scan(map, iter_ints, &vals2));
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

    hashmap_clear(map);
    assert(hashmap_count(map) == 0);


    for (unsigned i = 0; i < N; i++) {
        while (true) {
            assert(!hashmap_set(map, &vals[i]));
            if (!hashmap_oom(map)) {
                break;
            }
        }
    }

    hashmap_clear(map);

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

    hashmap_clear(map);
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
    return fail;
}
