/* from https://stackoverflow.com/questions/65217545 */
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

void * xcalloc(size_t count, size_t size);

struct btree {
	char v;
	char m[32];
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

static void
print_tree(struct btree *b, int w, int k, int diff)
{
	if( b ) {
		if( w < 0 ) {
			char *col = getenv("COLUMNS");
			w = col ? strtol(col, NULL, 0) / 2: 40;
			diff = w / 2;
			fputs("\033[2J", stdout); /* clear screen */
		}
		fprintf(stdout, "\033[%d;%dH", k, w);
		putc(b->v, stdout);
		print_tree(b->dot, w - diff, k + 1, diff / 2);
		print_tree(b->dash, w + diff, k + 1, diff / 2);
	}
	/*
	- put the cursor at line L and column C.  \033[<L>;<C>H   # tput cup $L $C
	- put the cursor at line L and column C.  \033[<L>;<C>f
	- Move the cursor up N lines:             \033[<N>A       # tput cuu $N
	- Move the cursor down N lines:           \033[<N>B       # tput cud $N
	- Move the cursor forward N columns:      \033[<N>C       # tput cuf $N
	- Move the cursor backward N columns:     \033[<N>D       # tput cub $N
	- Clear the screen, move to (0,0):        \033[2J
	- Erase to end of line:                   \033[K          # tput el
	- Save cursor position:                   \033[s          # tput sc
	- Restore cursor position:                \033[u          # tput rc
	*/
}

int
main(int argc, char **argv)
{
	int i;
	struct btree *root = NULL;
	(void)argc;
	/* char characters[] = "ETIANMSURWDKGOHVFLPJBXCYZQ"; */

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
	print_tree(root, -1, 1, 0);
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
