/* Walk a directory tree *without* using nftw or ftw
 * (see nftw.c for an example using nftw)
 * Print modification time of files.
 */

#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

int
list_dir(char *path)
{
	struct dirent *f;
	int rv = 0;
	char *e = strchr(path, '\0');
	DIR *d = opendir(path);

	if( d == NULL ){
		perror(path);
		return -1;
	}
	while( (f = readdir(d)) != NULL ){
		*e = '/';
		strncpy(e + 1, f->d_name, PATH_MAX - 1 - (e - path));
		if( ! strcmp(".", f->d_name) || ! strcmp("..", f->d_name) ){
			continue;
		}
		if( f->d_type == DT_DIR ){
			list_dir(path);
		}

		struct stat s;
		char *t;
		if( lstat(path, &s) ){
			perror(path);
			rv = -1;
			break;
		}
		t = ctime(&s.st_mtimespec.tv_sec);
		if( 0 >
			printf("%s%c\t%s", f->d_name,
				f->d_type == DT_DIR ? '/' :
				f->d_type == DT_LNK ? '@' :
				f->d_type == DT_FIFO ? '|' :
				f->d_type == DT_SOCK ? '=' :
				' ',
				t
			)
		){
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
	char name[PATH_MAX];
	char *defaults[] = { ".", NULL };
	char **path = argc > 1 ? argv + 1 : defaults;

	for( ; *path; path += 1 ){
		strncpy(name, *path, sizeof name);
		if( list_dir(name) ){
			return 1;
		}
	}
	return 0;
}


