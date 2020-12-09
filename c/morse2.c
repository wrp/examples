/* from https://stackoverflow.com/questions/65217545 */
#include <stdlib.h>
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
main(void)
{
	int i;
	struct btree *root = NULL;
	/* char characters[] = "ETIANMSURWDKGOHVFLPJBXCYZQ"; */

	insert(&root, alphamorse[4], 'A' + 4);
	for(i = 0; i < 26; i++ ){
		insert(&root, alphamorse[i], 'A' + i);
	}
	for(i = 0; i < 10; i++ ){
		insert(&root, nummorse[i], '0' + i);
	}
	preorder(root);
	putchar('\n');
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
