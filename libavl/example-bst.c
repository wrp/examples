#include "bst.h"
#include <string.h>
#include <stdio.h>


struct person {
	char *name;
	int age;
	char *occupation;
};


int
comp(const void *a, const void *b, void *u)
{
	(void)u;
	return strcmp(((struct person *)a)->name,((struct person *)b)->name);
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
	return 0;
}
