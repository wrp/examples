/*
 * Store words in input in a hash table, growing as needed
 * keywords: hashmap, hash, hashtable
 */
#define _GNU_SOURCE  /* For re-entrant verisions */
#include <err.h>
#include <errno.h>
#include <limits.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct word {
	char *d;
	struct word *next;
};

static void
push(struct word **root, char *d)
{
	struct word *n = malloc(sizeof *n);
	if( n == NULL ){
		perror("malloc");
		exit(1);
	}
	n->d = d;
	n->next = *root;
	*root = n;
}


/* Create a new table that is twice the size and rehash */
static void
grow(struct hsearch_data *table, unsigned long *size, struct word *words)
{
	struct hsearch_data new = {0};
	if( ! hcreate_r( (*size) *= 2, &new) ){
		err(1, "grow to %d", *size);
	}
	for( ; words; words = words->next ){
		ENTRY item;
		ENTRY *e;
		item.key = words->d;

		if( hsearch_r(item, FIND, &e, table) ){
			if( hsearch_r(*e, ENTER, &e, &new) == 0 ){
				err(1, "unexpected error growing hashmap");
			}
		} else {
			err(1, "%s not found", words->d);
		}
	}
	hdestroy_r(table);
	*table = new;
}



/* Insert *item into *table, growing *table if needed. */
static void
insert(ENTRY *item, struct hsearch_data *table, unsigned long *size,
	struct word *words)
{
	ENTRY *old;
	while( hsearch_r(*item, ENTER, &old, table) == 0 ){
		if( errno == ENOMEM ){
			grow(table, size, words);
		} else {
			perror("hsearch");
			exit(1);
		}
	}
}

int
main(int argc, char **argv)
{
	struct hsearch_data table = {};
	char str[512];
	struct word *words = NULL;
	unsigned long max = argc > 1 ? strtoul(argv[1], NULL, 10) : 65536;

	ENTRY item;

	if( ! hcreate_r(max, &table) ){
		perror("hcreate");
		return 1;
	}

	while( scanf("%511s", str) == 1 ){
		item.key = str;
		ENTRY *this;

		if( hsearch_r(item, FIND, &this, &table) ){
			*(int *)this->data += 1;
		} else {
			int *x = malloc(sizeof *x);
			if( x == NULL ){
				perror("malloc");
				exit(1);
			}
			item.key = strdup(str);
			item.data = x;
			*x = 1;
			insert(&item, &table, &max, words);
			push(&words, item.key);
		}
	}

	/* Access table. */
	for( ; words; words = words->next ){
		item.key = words->d;
		ENTRY *e;
		if( hsearch_r(item, FIND, &e, &table) ){
			printf("%s: %d\n", words->d, *(int *)e->data);
		} else {
			fprintf(stderr, "ERROR: %s not found\n", words->d);
			return 1;
		}
	}
	hdestroy_r(&table);
	return 0;
}
