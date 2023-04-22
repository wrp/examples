/* Store words in input in a hash table */
/* keywords: hashmap, hash, hashtable */
#define _GNU_SOURCE
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
			push(&words, item.key);
			item.data = x;
			*x = 1;
			ENTRY *old;
			if( hsearch_r(item, ENTER, &old, &table) == 0 ){
				perror("hsearch");
				return 1;
			}
		}
	}

	/* Access table. */
	for( ; words; words = words->next ){
		item.key = words->d;
		ENTRY *e;
		if( hsearch_r(item, FIND, &e, &table) ){
			printf("%s: %d\n", words->d, *(int *)e->data);
		} else {
			printf("%s was not found\n", words->d);
		}
	}
	hdestroy();
	return 0;
}
