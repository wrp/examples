/* Import numbers into a doubly linked list */
#include<stdio.h>
#include<stdlib.h>
struct node {
	int val;
	struct node *next, *prev;
};

void *xmalloc(size_t s);

/*
 * Allocate a new struct node and push it to the end of the list.
 */
void
push(struct node *list, int val)
{
	struct node *new = xmalloc(sizeof *list->next);
	new->val = val;
	new->prev = list->prev;
	list->prev = new;
	new->prev->next = new;
	new->next = list;
}

/*
 * Print the contents of the list.
 */
void
display(const struct node *list, const char *fmt)
{
	for(const struct node *t = list->next; t != list; t = t->next) {
		printf(fmt, t->val);
	}
}

int
main(int argc, char **argv)
{
	struct node list;
	int val;
	const char delim = argc > 1 ? argv[1][0] : ' ';
	char fmt[32];

	snprintf(fmt, sizeof fmt, "%%i%c", delim);
	list.next = &list;
	list.prev = &list;

	while( scanf(fmt, &val) == 1) {
		push(&list, val);
	}
	display(&list, fmt);
}

void *
xmalloc(size_t s)
{
	void *rv = malloc(s);
	if(rv == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return rv;
}
