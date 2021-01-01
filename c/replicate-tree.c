/* Recursively duplicate a directory tree */
#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static DIR * xopendir(const char *);
static void xclosedir(DIR *, const char *);
static void xstat(const char *, struct stat *);
static void xrealpath(const char *s, char *resolved);
static void pathcat(char *p, char *n);

struct dir { DIR *d; char *path, *end; };

static void
build_dir(char *spath, char *dpath, char *name, mode_t mod)
{
	struct dirent *f;
	struct dir src;
	struct dir dst;

	src.end = strchr(spath, '\0');
	dst.end = strchr(dpath, '\0');

	pathcat(dpath, name);
	if( mkdir(dpath, mod) == -1 && errno != EEXIST ){
		err(EXIT_FAILURE, "%s", name);
	}

	if( getenv("V") ){
		printf("%s -> %s\n", spath, dpath);
	}
	src.d = xopendir(src.path = spath);

	while( (f = readdir(src.d)) != NULL ) {
		if( !strcmp(f->d_name, ".") || !strcmp(f->d_name, "..") ){
			continue;
		}
		if( f->d_type == DT_DIR ){
			struct stat b;
			pathcat(spath, f->d_name);
			xstat(spath, &b);
			build_dir(spath, dpath, f->d_name, b.st_mode);
			*src.end = '\0';
		}
	}
	*src.end = '\0';
	*dst.end = '\0';
	xclosedir(src.d, spath);
}


int
main(int argc, char **argv)
{
	if( argc < 2 ){
		errx(EXIT_FAILURE, "src directory missing");
	}
	if( argc < 3 ){
		errx(EXIT_FAILURE, "target directory missing");
	}
	char spath[PATH_MAX];
	char dpath[PATH_MAX];
	xrealpath(argv[1], spath);

	for( argv += 2; *argv; argv++ ){
		char *name = *argv;
		if( *name == '/' ){
			dpath[0] = '\0';
		} else {
			getcwd(dpath, sizeof dpath);
		}
		build_dir(spath, dpath, name, 0777);
	}
	return 0;
}

static void
xrealpath(const char *s, char *resolved)
{
	if( realpath(s, resolved) == NULL ){
		err(EXIT_FAILURE, "%s", s);
	}
}

static DIR *
xopendir(const char *path)
{
	DIR *d = opendir(path);
	if( d == NULL ) {
		err(EXIT_FAILURE, "%s", path);
	}
	return d;
}

static void
xstat(const char *path, struct stat *buf)
{
	if( stat(path, buf) ){
		err(EXIT_FAILURE, "stat %s", path);
	}
}

static void
xclosedir(DIR *d, const char *path)
{
	if( closedir(d) ) {
		err(EXIT_FAILURE, "%s", path);
	}
}

static void
pathcat(char *p, char *n)
{
	if( snprintf(p, PATH_MAX, "%s/%s", p, n) > PATH_MAX - 1 ){
		errx(1, "path too long");
	}
}
