

/*
     void *
     tdelete(const void *restrict key, void **restrict rootp, int (*compar) (const void *key1, const void *key2));

     void *
     tfind(const void *key, void *const *rootp, int (*compar) (const void *key1, const void *key2));

     void *
     tsearch(const void *key, void **rootp, int (*compar) (const void *key1, const void *key2));

     void
     twalk(const void *root, void (*action) (const void *node, VISIT order, int level));
*/


#include <search.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

struct wc {
	char *word;
	int count;
};

void print_word(const void *a, VISIT o, int l)
{
	struct wc *const*aw = a;
	printf("%s: %d\n", (*aw)->word, (*aw)->count);
}
int compar(const void *k1, const void *k2)
{
	const struct wc *a = k1;
	const struct wc *b = k2;
	return strcmp(a->word, b->word);
}

int
main(int argc, char **argv)
{
	char buf[256];
	void *root = NULL;

	while( scanf("%255s", buf) == 1 ){
		struct wc w = { buf, 0 };
		struct wc **t;
		if( ( t = tfind(&w, &root, compar)) != NULL ){
			(*t)->count += 1;
		} else {
			struct wc *k = malloc(sizeof *t);
			if( k == NULL ){
				perror("malloc");
				exit(1);
			}
			k->word = strdup(buf);
			k->count = 1;
			tsearch(k, &root, compar);
		}
	}
	twalk(root, print_word);
	return 0;
}
