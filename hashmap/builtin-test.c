#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "hashmap.h"

int fail = 0;  /* Count of failed tests */
#define assert(x) do { if( !(x) ){ \
	fail += 1; \
	fprintf(stderr, "Test failure in %s at line %d: %s\n", \
		__FILE__, __LINE__, #x); \
	} } while(0)

static int rand_alloc_fail_odds = 3; // 1 in 3 chance malloc will fail.
static unsigned total_allocs = 0;
static unsigned total_mem = 0;

static void *
xmalloc(size_t size)
{
	size_t *header;
	if( rand() % rand_alloc_fail_odds == 0 ){
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
hash_int(const void *item, const void *seeds)
{
	const struct { int a, b; } *s = seeds;
	return hashmap_murmur(item, sizeof(int), s->a);
}

static uint64_t
hash_str(const void *vitem, const void *seeds)
{
	char *item = *(char **)vitem;
	const struct { int a, b; } *s = seeds;
	return hashmap_murmur(item, strlen(item), s->a);
}

static void
free_str(void *item) {
	xfree(*(char**)item);
}

/* Test some specific values. */
static void
test_exact_hashes(void)
{

	assert( hashmap_sip("hello", 5, 1, 2) == 2957200328589801622 );
	assert( hashmap_murmur("hello", 5, 1) == 1682575153221130884 );

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
		assert( hashmap_murmur("hello", 5, s) == expect_murmur[i]);
	}

	for( int i = 0; i < 64; i += 1 ){
		uint64_t s = 0x0000000000000005 << i;
		uint64_t t = 0xa000000000000000 >> i;
		char msg[32];
		for( int j = 0; j < 32; j += 1 ){
			msg[j] = i + j;
		}
		uint64_t expect_sip[] = {
			0x8bcd2344a381434b,
			0xac3cb54f24a54788,
			0x54d39dad944cca2e,
			0x97a28e41062f5c60,
			0xcf7ae54aa83bbfda,
			0x8d9d3e7da0239238,
			0x3259d5612c614fc2,
			0x7b109de9cc5b9eb7,
			0x3ba803d6250eb9f4,
			0x3ea3254d677cf79a,
			0xe9621de57cfda67e,
			0xf710a7b2602e48db,
			0x8bef5c04c2ef2105,
			0x10d73e9a111c1815,
			0x4b8bca55647be9bf,
			0x64d7bc6ef2a6f9b1,
			0x50f2e385bc56672f,
			0xa3b8128e6dc335fb,
			0x5eab036fd7b84a02,
			0xfe038eae96415fbc,
			0x5c87047cae8f265d,
			0x0ff5a584b4ebe373,
			0xa58fd058a8291861,
			0x1f743f661a823c2d,
			0x57d46a3155eedccb,
			0x3b165bf27a0eeadd,
			0x537c533b40b77eae,
			0xdcb66867286ab486,
			0x83812829b779f86c,
			0x7bcc9690de93d10f,
			0x1302d2671a9035b6,
			0x6638062dd1d96589,
			0xdf8710096a75ba2f,
			0x8f907370ea52cd1d,
			0xfcd94409f8341359,
			0x23bf22307706efb8,
			0x3b4296fcce08c34f,
			0xabbbf03d5cec769a,
			0x811efc819bb9aee4,
			0xebd42a9812926c0a,
			0x666b3c47bb7172c9,
			0xb9fac09cc4c68b31,
			0x3b62582cf2b8bdde,
			0x0a36342f92143190,
			0x48dc2fed12ff4341,
			0x2a79c83e0cf0b156,
			0xc0c53eccd0288202,
			0xda83ed0ee5e493c9,
			0x0f66e90de9496dc2,
			0x927542cf995ac1d6,
			0x5f4cf9038bf5e40c,
			0x1408072d1b85c5ee,
			0x12b1b664e1bc0e3e,
			0x1fbfb49708e47a60,
			0x496b4954fe8b6e8b,
			0xf7f7fd969ca06500,
			0xbbf3676acbb98f8c,
			0x116279ff49ec0d85,
			0x01473bb8fd554f86,
			0xfd89e984281d5c88,
			0x48e1901ad3d26b27,
			0x880ddd83d3dcb0ba,
			0xc55bbd931bcd6226,
			0x6aceff7ff67b2cb0
		};
		uint64_t expect_murmur[] = {
			0xa50ba8665ccb5fd7,
			0x771fb70358a732f0,
			0xc29c27141a0f2051,
			0x12fefe98a7f05270,
			0xa8f227132e62c3c9,
			0x55ebc770a28c2796,
			0x57cbf89cbddf92dd,
			0x9afe793b7fe13f1e,
			0xdd15497c77e50070,
			0x5a7526f7073164ac,
			0xe4b50e9b19c4a5cf,
			0xbc6f5cfcb2df1b1c,
			0x3bb52e709798c863,
			0xcb3cbe3180f4073b,
			0xf7cebc01e6e2f238,
			0xd18cd5eaf2c76176,
			0x60e4bbf876c10f6f,
			0x2928cba9eed718f4,
			0x651a8034fc5a3707,
			0xbd042db0621ae6ab,
			0x1d4bc3cce583a4f4,
			0x34d0354e840df600,
			0x6d8519940b650fe6,
			0x0e9976aeb0365b40,
			0xca7cd5f6b786978e,
			0x6f3ec7bfb815757e,
			0xc9910a01edb09c39,
			0x830440dfba487831,
			0xf07f19e32735324f,
			0x8c41fd423caff9c9,
			0x8566c3ca7d64e714,
			0x9c660a8861041bc5,
			0xee60e616ea24583d,
			0x8c35a9657ac6cdd5,
			0x6339f24e1e503733,
			0xa4428a76d7415fdb,
			0x5b333b7bfd8972bf,
			0xdebb9af0d9d200de,
			0xdd35b540841a01d8,
			0x47b50ed5742a6686,
			0xa091c6fe6d63bed9,
			0x72908badcebe43c5,
			0x7b64b329d5519bee,
			0xa840335d03c6c1ac,
			0x0123caa1520383c8,
			0x8bfbe30109310698,
			0xca78ac7bae53350d,
			0x6039823f150c4294,
			0xe16c711478b7c4de,
			0x14065fc8893be8c8,
			0xdd152b41839e420c,
			0x364a70190a59fab6,
			0xcde0828007c2375d,
			0xf67fd0603b14fc4d,
			0xb01444c182c53cf5,
			0x877ca4eb2c5dda35,
			0xb20e166d000c2b3a,
			0xc25dab79ff3142de,
			0x2c239faccf06cfd8,
			0x853e5a4a68331eb0,
			0x7a38cb648b7f8388,
			0xf166b51f829e900f,
			0xaddb3b25460ad454,
			0xd674885031a29485
		};

		assert( hashmap_sip(msg, 32, s, t) == expect_sip[i] );
		assert( hashmap_murmur(msg, 32, s) == expect_murmur[i] );
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
		assert( hashmap_murmur(msg, 5, 1) == 0x1759b52feba4da84 );
	}
}

/* exercise the scan */
static void
test_scan(unsigned N, struct hashmap *map)
{
	int *v;

	do v = xmalloc(N * sizeof *v);
	while( v == NULL);

	memset(v, 0, N * sizeof *v);
	assert( 0 == hashmap_scan(map, iter_ints, &v) );
	for( unsigned i = 0; i < N; i += 1 ){
		assert(v[i] == 1);
	}
	xfree(v);
}

static void
test_1(unsigned N, unsigned seed)
{
	int *vals;
	do vals = xmalloc(N * sizeof *vals);
	while( vals == NULL );

	for( unsigned i = 0; i < N; i += 1 ){
		vals[i] = (int)i;
	}

	struct { int a, b; } seeds = { seed, seed };
	struct hashmap *map;
	struct hash_element el = { sizeof *vals, compare_ints_udata };

	do map = hashmap_new_with_allocator(xmalloc, xfree, &el,
		hash_int, &seeds, 0);
	while( map == NULL );

	shuffle(vals, N, sizeof(int));

	for( unsigned i = 0; i < N; i += 1 ){
		assert(i == hashmap_count(map));

		int *v;
		int *t = vals + i;
		assert( ! hashmap_get(map, t) );
		assert( ! hashmap_delete(map, t) );

		/* Insert the value into the map, looping to allow
		 * for random malloc failures */
		do assert( !hashmap_set(map, t) );
		while( hashmap_oom(map) );

		for( unsigned j = 0; j < i; j += 1 ){
			int *tj = vals + j;
			v = hashmap_get(map, tj);
			assert( v && *v == *tj );
		}

		do {
			v = hashmap_set(map, t);
			if( !v ){
				assert( hashmap_oom(map) );
			} else {
				assert( !hashmap_oom(map) );
				assert( v && *v == *t );
			}
		} while( hashmap_oom(map) );

		v = hashmap_get(map, t);
		assert( v && *v == *t );
		v = hashmap_delete(map, t);
		assert( v && *v == *t );
		assert( !hashmap_get(map, t) );
		assert( !hashmap_delete(map, t) );
		assert( !hashmap_set(map, t) );
		assert( i + 1 == hashmap_count(map) );
	}

	test_scan(N, map);

	shuffle(vals, N, sizeof *vals);
	for( unsigned i = 0; i < N; i += 1 ){
		int *t = vals + i;
		int *v;
		v = hashmap_delete(map, t);
		assert( v && *v == *t );
		assert( !hashmap_get(map, t) );
		assert( N - i - 1 == hashmap_count(map) );
		for( unsigned j = N - 1; j > i; j -= 1 ){
			v = hashmap_get(map, vals + j);
			assert( v && *v == vals[j] );
		}
	}

	for( unsigned i = 0; i < N; i += 1 ){
		do assert( !hashmap_set(map, vals + i) );
		while( hashmap_oom(map) );
	}

	assert( hashmap_count(map) == N );
	hashmap_clear(map);
	assert( hashmap_count(map) == 0 );


	for( unsigned i = 0; i < N; i += 1 ){
		do assert( !hashmap_set(map, vals + i) );
		while( hashmap_oom(map) );
        }

    hashmap_clear(map);
    hashmap_free(map);
    xfree(vals);
}


static void
populate_map(struct hashmap *map, unsigned N)
{
	for( unsigned i = 0; i < N; i += 1 ){
		char *str;
		void *v;
		do str = xmalloc(16);
		while( str == NULL );
		snprintf(str, 16, "s%i", i);
		do v = hashmap_set(map, &str);
		while( v == NULL);
	}
}

int
main(int argc, char **argv)
{
	unsigned N = argc > 1 ? strtoul(argv[1], NULL, 10) : 2000;
	unsigned seed = argc > 2 ? strtoul(argv[2], NULL, 10) : time(NULL);

	printf("seed=%d, count=%d, item_size=%zu\n", seed, N, sizeof(int));
	srand(seed);
	struct { int a, b; } seeds = { seed, seed };

	test_exact_hashes();

	test_1(N, seed);

	struct hashmap *map;
	struct hash_element el = { sizeof(char *), compare_strs, free_str };

	do map = hashmap_new_with_allocator( xmalloc, xfree, &el, hash_str,
		&seeds, 0);
	while( map == NULL );

	populate_map(map, N);

	unsigned allocs = total_allocs;
	hashmap_clear(map);
	assert( total_allocs == allocs - N );

	assert( hashmap_count(map) == 0 );
	populate_map(map, N);

	hashmap_free(map);

	assert( total_allocs == 0 );
	if( fail > 0 ){
		fprintf(stderr, "%d total test failures\n", fail);
	}
	return !!fail;
}
