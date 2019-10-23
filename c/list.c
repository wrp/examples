/*
 * Trivial demonstration of a linked list
 * and a simple (unbalanced) binary search tree.
 */
#include<stdio.h>
#include<stdlib.h>
struct node {
	int val;
	struct node *next, *prev;
};

struct tree {
	int val;
	struct tree *left, *right;
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
	putchar('\n');
}

/*
 * Push val into the tree.  Return non-zero if val
 * is already in the tree
 */
int
push_tree(struct tree **t, int val)
{
	struct tree *T = *t;
	if( T == NULL ) {
		T = *t = xmalloc(sizeof **t);
		T->val = val;
		T->left = T->right = NULL;
		return 0;
	} else if( T->val == val) {
		return 1;
	} else if( T->val < val) {
		return push_tree( &(T->left), val);
	} else {
		return push_tree( &(T->right), val);
	}
}

int
main(int argc, char **argv)
{
	struct tree *tree = NULL;
	struct node list = {0, &list, &list};
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

	/* delete duplicates */
	for(struct node *t = list.next; t != &list; t = t->next) {
		if(push_tree(&tree, t->val)) {
			struct node *tmp = t;
			t->prev->next = t->next;
			t->next->prev = t->prev;
			t = t->prev;
			free(tmp);
		}
	}
	display(&list, fmt);
}

void *
xmalloc(size_t s)
{
	void *rv = malloc(s);
	if(rv == NULL) {
		perror("malloc"); /* uncovered */
		exit(EXIT_FAILURE); /* uncovered */
	}
	return rv;
}
