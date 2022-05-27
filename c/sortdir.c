/*
 * Sort regular files in a directory by size
 */

#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

struct entry {
	char *name;
	size_t siz;
};

struct entries {
	struct entry *data;
	size_t cap;
	size_t len;
};

static void
push(struct entries *e, char *name, size_t siz)
{
	while( e->cap <= e->len ){
		e->data = realloc(e->data, sizeof *e->data * (e->cap += 128));
		if( e->data == NULL ){
			perror("realloc");
			exit(EXIT_FAILURE);
		}
	}
	struct entry *t = e->data + e->len++;
	t->name = strdup(name);
	t->siz = siz;
	if( t->name == NULL ){
		perror("strdup");
		exit(EXIT_FAILURE);
	}
}


int
comp(const void *va, const void *vb)
{
	const struct entry *a = va;
	const struct entry *b = vb;
	return a->siz - b->siz;
}

int
sort_dir(char *path, int print_name)
{
	struct dirent *f;
	struct entries ent = {0};
	int rv = 0;
	char *e = strchr(path, '\0');
	DIR *d = opendir(path);

	if( print_name ){
		printf("%s:\n", path);
	}

	if( d == NULL ){
		perror(path);
		return -1;
	}
	while( (f = readdir(d)) != NULL ){
		if( f->d_type != DT_REG ){
			continue;
		}
		struct stat s;
		char *t;
		*e = '/';
		strncpy(e + 1, f->d_name, PATH_MAX - 1 - (e - path));
		if( lstat(path, &s) ){
			perror(path);
			rv = -1;
			break;
		}
		push(&ent, f->d_name, s.st_size);
	}
	qsort(ent.data, ent.len, sizeof *ent.data, comp);
	for( struct entry *t = ent.data; t < ent.data + ent.len; t++ ){
		printf("%20s: %zu\n", t->name, t->siz);
	}

	*e = '\0';
	if( closedir(d) ) {
		perror(path);
		rv = -1;
	}
	return rv;
}


int
main(int argc, char **argv)
{
	/* Copy paths to a writeable buffer so we can append names */
	char name[PATH_MAX];
	char *defaults[] = { ".", NULL };
	char **path = argc > 1 ? argv + 1 : defaults;

	for( ; *path; path += 1 ){
		strncpy(name, *path, sizeof name);
		if( sort_dir(name, argc > 1) ){
			return 1;
		}
	}
	return 0;
}
