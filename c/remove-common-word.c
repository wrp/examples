/*
 * Remove most common word from the input stream
 * Motivated by https://stackoverflow.com/questions/72758127
 */

#include <ctype.h>
#include <search.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

struct offt_buf {
	off_t *data;
	size_t count;
	size_t cap;
};
struct word {
	char *text;
	size_t len;
	struct offt_buf position;
};
struct char_buf {
	char *data;
	size_t cap;
	size_t len;
};

void *xrealloc(void *, size_t, size_t);
void char_push(struct char_buf *k, int c);
void off_push(struct offt_buf *k, off_t c);

struct word *
get_word(char *str)
{
	ENTRY item;
	item.key = str;
	ENTRY *this = hsearch(item, FIND);

	struct word *w;

	if( this ){
		w = this->data;
	} else {
		w = item.data = xrealloc(NULL, 1,  sizeof *w);
		w->text = item.key = str;
		w->len = strlen(str);
		w->position.data = NULL;
		w->position.count = 0;
		w->position.cap = 0;
		if( hsearch(item, ENTER) == NULL ){
			perror("hsearch");
			exit(EXIT_FAILURE);
		}
	}
	return w;
}

int
main(int argc, char **argv)
{
	struct char_buf i = {0};
	size_t word_start = 0;
	int c, p = ' ';
	struct word *most_frequent = NULL;

	if( ! hcreate(65536) ){
		perror("hcreate");
		return EXIT_FAILURE;
	}

	while( (c = getchar()) != EOF ){
		if( isspace(c) && ! isspace(p)){
			char_push(&i, '\0');

			struct word *w = get_word(i.data + word_start);
			off_push(&w->position, word_start);
			if( most_frequent == NULL || w->position.count >
					most_frequent->position.count ){
				most_frequent = w;
			}
		}
		if( ! isspace(c) && isspace(p) ){
			word_start = i.len;
		}
		char_push(&i, p = c);
	}
	struct word *m = most_frequent;
	int k = 0;
	for( char *out = i.data; out < i.data + i.len; out += 1 ){
		if( out - i.data == m->position.data[k] ){
			out += m->len;
			k += 1;
		}
		if( out < i.data + i.len && *out ){
			putchar(*out);
		}
	}

	return 0;
}


void
char_push(struct char_buf *k, int c)
{
	while( k->len >= k->cap ){
		k->data = xrealloc(k->data, k->cap += 1024, sizeof *k->data);
	}
	k->data[k->len++] = c;
}

void
off_push(struct offt_buf *k, off_t c)
{
	while( k->count >= k->cap ){
		k->data = xrealloc(k->data, k->cap += 1024, sizeof *k->data);
	}
	k->data[k->count++] = c;
}


void *
xrealloc(void *b, size_t num, size_t siz)
{
	b = realloc(b, num * siz);
	if( b == NULL ){
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	return b;
}
