/*
 * Trivial demonstration of a linked list
 * and a simple (unbalanced) binary search tree.
 *
 * Note: this uses a sentinel object for the list (eg,
 * a node that acts as a HEAD object that is not part
 * of the list. This allows us to avoid checking for NULL
 * in the list operators.)
 */
#include<stdio.h>
#include<stdlib.h>
#include "xutil.h"

struct node {
	int val;
	struct node *next, *prev;
};

struct tree {
	int val;
	struct tree *left, *right;
};


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
 * Reverse the list
 */
void
reverse(struct node *list)
{
	struct node *t = list;
	do {
		struct node *next = t->next;
		t->next = t->prev;
		t = t->prev = next;
	} while( t != list );
}


/*
 * Print the contents of the list.
 */
void
display(const struct node *list, char delim)
{
	char *cols = getenv("COLUMNS");
	int avail = cols ? strtol(cols, NULL, 10) : 80;
	int used = 0;
	for(const struct node *t = list->next; t != list; t = t->next) {
		used += printf("%i%c", t->val,
			t->next == list ? '\n'
			: avail - used < 20 ? used = 0, '\n'
			: delim
		);
	}
}

/*
 * Push val into the tree.  Return non-zero if val
 * is already in the tree.
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
	char delim = argc > 1 ? argv[1][0] : ' ';
	char fmt[32];

	snprintf(fmt, sizeof fmt, "%%i%c", delim);
	list.next = &list;
	list.prev = &list;

	while( scanf(fmt, &val) == 1) {
		push(&list, val);
	}

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
	display(&list, delim);
	reverse(&list);
	display(&list, delim);
	return EXIT_SUCCESS;
}
