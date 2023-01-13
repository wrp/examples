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
FILE * xfopen(const char *path, const char *mode);

typedef struct string{ char *start, *end; size_t cap; } string;
typedef struct int_buffer { int *start, *end; size_t cap; } int_buffer;

struct entry {
	char *word;
	int count; /* Number of times this word appears */
	struct int_buffer lines; /* Lines in which the word appears */
	struct entry *node[2];
};

#define expand(T) static void \
	push_##T(int c, T *b) \
	{ \
		if( b->start == NULL || b->end >= b->start + b->cap ) { \
			b->start = xrealloc(b->start, b->cap += 128, \
				sizeof *b->start, &b->end); \
		} \
		*b->end++ = c; \
	}
expand(int_buffer)
expand(string)
#undef expand
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

/* Find an entry in the table, or insert if not present.  */
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
	char **e = endvp, *s = buf;
	ptrdiff_t offset = s && e && *e ? *e - s : 0;

	/* Without this printf, getting segfaults with -O2.  TODO */
	/* printf(stderr, "num = %zd\n", num); */
	s = realloc(s, num * siz);
	if( s == NULL ){
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	if( e != NULL ){
		*e = s + offset;
	}
	return s;
}

int (*is_word)(int) = isalnum;

static int
get_word(struct string *w, FILE *ifp)
{
	int b, c;
	int rv = 0;
	while( (b = c = fgetc(ifp)) != EOF && is_word(c)){
		push(tolower(c), w);
		rv = 1;
	}
	/* Handle some edge cases. */
	if( strchr("-,'", b) ) {
		c = fgetc(ifp);
		if( b == '-' && c == '\n' ) {
			/* hyphen at end of word */
			return get_word(w, ifp);
		} else if(
			(b == '-' && is_word(c)) /* embedded hyphen in a word */
			|| (b == ',' && isdigit(index_s(w, -1)) && isdigit(c) )
			|| (b == '\'' && isalpha(c))
		){
			push(b, w);
			ungetc(c, ifp);
			return get_word(w, ifp);
		}
	}
	push('\0', w);
	if( c != EOF ){
		ungetc(c, ifp);
	}
	return rv;
}

/* Discard all input that is not a word. */
static int
skip_non(int *line, FILE *ifp)
{
	int c;
	while( (c = fgetc(ifp)) != EOF && !is_word(c) ){
		if( c == '\n' ) {
			*line += 1;
		}
	}
	if( c != EOF ){
		ungetc(c, ifp);
	}
	return c != EOF;
}

int
main(int argc, char **argv)
{
	FILE *ifp = argc > 1 ? xfopen(argv[1], "r") : stdin;
	struct string word = {0};
	struct entry *table = NULL;
	int line = 1;
	while( skip_non(&line, ifp) && get_word(&word, ifp) ){
		process_word(&table, line, &word);
		word.end = word.start;
	}
	print_table(table);
}

FILE *
xfopen(const char *path, const char *mode)
{
	FILE *fp = path[0] != '-' || path[1] != '\0' ? fopen(path, mode) :
		*mode == 'r' ? stdin : stdout;
	if( fp == NULL ){
		perror(path);
		exit(EXIT_FAILURE);
	}
	return fp;
}
