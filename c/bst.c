/* Build an (unbalanced) binary search tree of words in input. */

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void * xrealloc(void *buf, size_t num, size_t siz, void *end);

typedef struct string{ char *start, *end; size_t cap; } string;
typedef struct int_buffer { int *start, *end; size_t cap; } int_buffer;

struct entry {
	char *word;
	int count; /* Number of times this word appears */
	struct int_buffer lines; /* Lines in which the word appears */
	struct entry *node[2];
};

#define T int_buffer
#include "push.c"
#define T string
#include "push.c"

#define push(c, X) _Generic((X),        \
	struct int_buffer *: push_int_buffer, \
	struct string *: push_string \
)(c, X)


static int
index_s(struct string *s, int i)
{
	if( i >= 0 ) {
		return s->start + i < s->end ? s->start[i] : EOF;
	} else {
		return s->start <= s->end + i ? s->end[i] : EOF;
	}
}

static struct entry *
new_node(const char *word, ptrdiff_t len)
{
	struct entry *e = calloc(1, sizeof *e);
	if( e == NULL ){
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	e->word = strndup(word, len);
	return e;
}

/* Find an entry in the table, or insert if not present.
 * Note that this tree needs to be rebalanced.  In a real
 * project, we would use existing libraries.  For this toy
 * it is not worth the work needed to properly rebalance the
 * tree.  Should probably use a hash map from C++, but I
 * wanted to do this in C to remind myself of the ...well,
 * maybe I'm just a massochist. :)  We will leave the tree
 * unbalanced and suffer the worst case linear lookup
 */
static struct entry *
lookup(struct entry **table, const char *word, ptrdiff_t len)
{
	struct entry *t = *table;
	if( t ) {
		int cmp = strncasecmp(word, t->word, len);
		if( cmp == 0 ) {
			return t;
		} else {
			return lookup(&t->node[cmp > 0], word, len);
		}
	} else {
		return *table = new_node(word, len);
	}
}

static void
process_word(struct entry **table, int line, const struct string *w)
{
	struct entry *e = lookup(table, w->start, w->end - w->start);
	assert( e != NULL );
	e->count += 1;
	push(line, &e->lines);
}

/* In-order descent of the tree, printing one line per entry */
static void
print_table(const struct entry *t)
{
	if( t ) {
		print_table(t->node[0]);
		printf("%s: {%d:", t->word, t->count);
		for( int i = 0; i < t->count - 1; i++ ) {
			printf("%d,", t->lines.start[i]);
		}
		printf("%d}\n", t->lines.start[t->count - 1]);
		print_table(t->node[1]);
	}
}

static void *
xrealloc(void *buf, size_t num, size_t siz, void *endvp)
{
	void **endp = endvp;
	ptrdiff_t offset = endp && *endp ? *endp - buf : 0;
	buf = realloc(buf, num * siz);
	if( buf == NULL ){
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	if( endp != NULL ){
		*endp = buf + offset;
	}
	return buf;
}

int (*is_word)(int) = isalnum;

static int
get_word(struct string *w)
{
	int b, c;
	while( (b = c = getchar()) != EOF && is_word(c)){
		push(tolower(c), w);
	}
	/* Handle some edge cases. */
	if( strchr("-,'", b) ) {
		c = getchar();
		if( b == '-' && c == '\n' ) {
			/* hyphen at end of word */
			return get_word(w);
		} else if(
			(b == '-' && is_word(c)) /* embedded hyphen in a word */
			|| (b == ',' && isdigit(index_s(w, -1)) && isdigit(c) )
			|| (b == '\'' && isalpha(c))
		){
			push(b, w);
			push(tolower(c), w);
			return get_word(w);
		}
	}
	push('\0', w);
	if( c != EOF ){
		ungetc(c, stdin);
	}
	return c != EOF;
}

/* Discard all input that is not a word. */
static int
skip_non(int *line)
{
	int c;
	while( (c = getchar()) != EOF && !is_word(c) ){
		if( c == '\n' ) {
			*line += 1;
		}
	}
	if( c != EOF ){
		ungetc(c, stdin);
	}
	return c != EOF;
}

int
main(void)
{
	struct string word = {0};
	struct entry *table = NULL;
	int line = 1;
	while( skip_non(&line) && get_word(&word) ) {
		process_word(&table, line, &word);
		word.end = word.start;
	}
	print_table(table);
}
