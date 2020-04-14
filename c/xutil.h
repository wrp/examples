
#include <stdio.h>
#include <stdlib.h>

FILE * xfopen(const char *path, const char *mode);
void * xrealloc(void *buf, size_t nmemb, size_t size, void *iterator);
void * xmalloc(size_t);
void * xcalloc(size_t, size_t);
int die(const char *fmt, ... ) __attribute__ ((format (printf, 1, 2)));
int xopen(const char *path, int flags);
char * xstrdup(const char *s1);

