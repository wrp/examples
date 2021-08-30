/* Check if any integer on the input stream is a dup */

#include <stdio.h>
#include <stdlib.h>

struct node { int data; struct node *child[2]; };

static struct node *
new_node(int data)
{
	struct node *e = calloc(1, sizeof *e);
	if( e == NULL ){
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	e->data = data;
	return e;
}

/*
 * Insert a value into the tree. Return 1 if already present.
 * Note that this tree needs to be rebalanced.  In a real
 * project, we would use existing libraries.  For this toy
 * it is not worth the work needed to properly rebalance the
 * tree.
 */
int
insert(struct node **table, int data)
{
	struct node *t = *table;
	if( !t ){
		*table = new_node(data);
		return 0;
	}
	if( data == t->data ){
		return 1;
	}
	return insert(&t->child[data < t->data], data);
}

int
main(void)
{
	int rv, v;
	struct node *table = NULL;
	while( (rv = scanf("%d", &v)) == 1 ){
		if( insert(&table, v) ){
			fprintf(stderr, "%d is duplicated\n", v);
			return EXIT_FAILURE;
		}
	}
	if( rv != EOF ){
		fprintf(stderr, "Invalid input\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
