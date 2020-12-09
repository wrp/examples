/* from https://stackoverflow.com/questions/65217545 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

void * xcalloc(size_t count, size_t size);

struct btree {
	char v;
	struct btree *dot, *dash;
} btree, *tree_ptr;

struct btree *
insert(struct btree **root, char *morse, char v)
{
	struct btree *b = *root;

	if( b == NULL ){
		b = *root = xcalloc(1, sizeof **root);
	}
	if( *morse ){
		assert( morse[0] == '-' || morse[0] == '.' );
		b = insert( *morse == '-' ? &(*root)->dash : &(*root)->dot,
			morse + 1, v);
	}
	if( *morse == '\0' ){
		b->v = v;
	}
	return b;
}
void
preorder(struct btree *root)
{
	if( root ){
		printf("%c", root->v) ;
		preorder(root->dot);
		preorder(root->dash);
	}
}
void
postorder(struct btree *root)
{
	if( root ){
		postorder(root->dot);
		postorder(root->dash);
		printf("%c", root->v) ;
	}
}
char *alphamorse[] = {
	".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", /* A - H */
	"..", ".---", "-.-", ".-..", "--", "-.", /* I - M */
	"---", ".--.", "--.-", ".-.", "...", "-", /* N - T */
	"..-", "...-", ".--", "-..-", "-.--", "--.." /* W - Z */
};
char *nummorse[]={
	"-----", ".----", "..---", "...--", "....-",
	".....", "-....", "--...", "---..", "----."
};

int
lookup(struct btree *b, const char *s)
{
	if( strchr("-.", *s) == NULL ){
		return -1;
	}

	return *s ? lookup( *s == '-' ? b->dash : b->dot, s + 1) : b->v;
}

int
main(int argc, char **argv)
{
	int i;
	struct btree *root = NULL;
	(void)argc;
	/* char characters[] = "ETIANMSURWDKGOHVFLPJBXCYZQ"; */

	insert(&root, alphamorse[4], 'A' + 4);
	for(i = 0; i < 26; i++ ){
		insert(&root, alphamorse[i], 'A' + i);
	}
	for(i = 0; i < 10; i++ ){
		insert(&root, nummorse[i], '0' + i);
	}

	for( argv += 1; *argv; argv++ ){
		printf("%20s: ", *argv);
		int v = lookup(root, *argv);
		if( v == -1 ){
			printf("invalid");
		} else {
			putchar(v);
		}
		putchar('\n');
	}
	return 0;

}

void *
xcalloc(size_t count, size_t size)
{
	void *r = calloc(count, size);
	if( r == NULL ){
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	return r;
}
