/* Simple implementation of ls */
#include <stdio.h>
#include <dirent.h>

int
list_dir(const char *path)
{
	struct dirent *f;
	int rv = 0;
	DIR *d = opendir(path);

	if( d == NULL ) {
		perror(path);
		return -1;
	}
	while( (f = readdir(d)) != NULL ) {
		if( printf("%s\n", f->d_name) < 0 ) {
			perror("stdout");
			rv = -1;
			break;
		}
	}
	if( closedir(d) ) {
		perror(path);
		rv = -1;
	}
	return rv;
}


int
main(int argc, char **argv)
{
	char *defaults[] = { ".", NULL };
	char **path = argc > 1 ? argv + 1 : defaults;
	for(; *path; path += 1 ) {
		if(list_dir(*path)) {
			return 1;
		}
	}
	return 0;
}

