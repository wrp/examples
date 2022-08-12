#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "hashmap.h"


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
	map = hashmap_new_with_allocator(
		malloc, free,
		sizeof *vals,
		0,
		&hash,
		compare_ints_udata,
		NULL, NULL
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

	map = hashmap_new_with_allocator(
		malloc, free,
		sizeof *vals, N,
		&hash, compare_ints_udata,
		NULL, NULL
	);
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
