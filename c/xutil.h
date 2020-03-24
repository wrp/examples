
#include <stdio.h>
#include <stdlib.h>

FILE * xfopen(const char *path, const char *mode);
void * xrealloc(void *, size_t);
void * xmalloc(size_t);
int die(const char *fmt, ... ) __attribute__ ((format (printf, 1, 2)));
