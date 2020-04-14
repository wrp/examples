
/* movitvated by https://stackoverflow.com/questions/61206360/random-word-in-file-without-same-word-twice-c-programming */

#include "config.h"
#include <stdio.h>
#include <ctype.h>
#include "xutil.h"

int
keep(int i, int n)
{
	int r;
	if( i < n ) {
		return i;
	}
#if HAVE_ARC4RANDOM_UNIFORM
	if( ( r = arc4random_uniform(i)) < n)
#else
	if( ( r = rand() % i) < n)
#endif
	{
		return r;
	}
	/* TODO: check duplicates */
	return -1;
}

int
main(int argc, char **argv)
{
	int count = argc > 1 ? strtol(argv[1], NULL, 10) : 10;
	char **words;
	int c;
	char *word = NULL;   /* current input word */
	size_t capacity = 0; /* capacity of word */
	char *t = word;      /* current input char */
	int idx = 0;
	int inword = 0;

	words = xcalloc(count, sizeof *words);
	while( ( c = getchar()) != EOF ) {
		if( word == NULL || t >= word + capacity ) {
			word = xrealloc(word, capacity += 128, sizeof *word, &t);
		}
		if( isalnum(c) ) {
			*t++ = c;
			inword = 1;
			continue;
		} else {
			if(inword) {
				int i;
				*t++ = '\0';
				/* this is the first char after a word */
				if( (i = keep(idx++, count)) != -1 ) {
					words[i] = word;
					word = NULL;
					capacity = 0;
				}
				t = word;
			}
			inword = 0;
		}
	}
	for(int i = 0; i < count; i++) {
		printf("%d: %s\n", i, words[i]);
	}
}




