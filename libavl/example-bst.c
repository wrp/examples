#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct person {
	char *name;
	int age;
	char *occupation;
};

void *
xmalloc(size_t s)
{
	void *rv = malloc(s);
	if( rv == NULL ){
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return rv;
}

int
comp(const void *a, const void *b, void *u)
{
	(void)u;
	return strcmp(((struct person *)a)->name,((struct person *)b)->name);
}


void *
insert(struct bst_table *b, char *name, int age, char *occupation)
{
	struct person *k = xmalloc(sizeof *k);
	k->name = name;
	k->age = age;
	k->occupation = occupation;
	return bst_insert(b, k);
}

void *
get(struct bst_table *b, char *name)
{
	return bst_find(b, &(struct person){name});
}

int
main(int argc, char **argv)
{
	struct bst_table *b;
	struct person a = { "bob", 54, "teacher" };
	struct person c = { "bob" };
	struct person *p;

	b = bst_create(comp, NULL, NULL);

	bst_insert(b, &a);

	p = bst_find(b, &c);

	printf("%s -> %d -> %s\n", p->name, p->age, p->occupation);

	insert(b, "larry", 23, "none");
	insert(b, "bob", 14, "none");
	insert(b, "linda", 54, "none");

	p = get(b, "linda");
	printf("%s -> %d -> %s\n", p->name, p->age, p->occupation);

	return 0;
}
