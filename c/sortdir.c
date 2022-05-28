/*
 * Sort regular files in a directory by size
 */

#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

struct entry {
	char *name;
	size_t siz;
	struct entry *n[2];  /* Child nodes */
};

static void
insert(struct entry **root, char *name, size_t siz)
{
	if( *root ){
		struct entry *t = *root;
		insert(t->n + (t->siz < siz), name, siz);
	} else {
		struct entry *t = *root = malloc(sizeof *t);
		if( t == NULL ){
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		t->name = strdup(name);
		t->siz = siz;
	}
}

static void
display(void *v)
{
	struct entry *t = v;
	printf("%40s: %zu\n", t->name, t->siz);
}

static void
inorder(struct entry *e, void (*f)(void *))
{
	if( e ){ inorder(e->n[0], f); f(e); inorder(e->n[1], f); }
}

static void
postorder(struct entry *e, void (*f)(void *))
{
	if( e ){ postorder(e->n[0], f); postorder(e->n[1], f); f(e); }
}


int
sort_dir(char *path, int print_name)
{
	struct dirent *f;
	struct entry *root = NULL;
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
			continue;
		}
		insert(&root, f->d_name, s.st_size);
	}
	inorder(root, display);
	postorder(root, free);

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
		if( sort_dir(name, argc > 2) ){
			return 1;
		}
	}
	return 0;
}
