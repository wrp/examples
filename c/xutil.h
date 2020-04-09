
#include <stdio.h>
#include <stdlib.h>

FILE * xfopen(const char *path, const char *mode);
void * xrealloc(void *buf, size_t s, size_t count, void *iterator);
void * xmalloc(size_t);
int die(const char *fmt, ... ) __attribute__ ((format (printf, 1, 2)));
int xopen(const char *path, int flags);
char * xstrdup(const char *s1);

