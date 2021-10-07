/* Print the nth line of a file, using a prepared index */
#include "config.h"
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "xutil.h"

struct indexed_file {
	FILE *fp;
	const char *path;
	off_t *idx;
	size_t lines;  /* Number of lines in file */
	char *index;
};

static void read_index(struct indexed_file *ifp);
static void build_index(struct indexed_file *ifp);
static void open_indexed(const char *path, struct indexed_file *ifp);
void xfseek(struct indexed_file *, long);

int
main(int argc, char **argv)
{
	struct indexed_file ifp[1];

	if( argc < 2 ){
		char *base = strrchr(argv[0], '/');
		base = base ? base + 1 : argv[0];
		printf("usage: %s input-file [line-num ...]\n", base);
		return EXIT_SUCCESS;
	}
	open_indexed(argv[1], ifp);
	for( argv += 2; *argv; argv++ ){
		size_t line = strtol(*argv, NULL, 10);
		int c;
		xfseek(ifp, line);
		while( (c = fgetc(ifp->fp)) != EOF && c != '\n' ){
			putchar(c);
		}
		putchar('\n');
	}
}

static void
open_indexed(const char *path, struct indexed_file *ifp)
{
	ifp->fp = xfopen(path, "r");
	ifp->path = path;
	read_index(ifp);
}

static void
read_index(struct indexed_file *ifp)
{
	int index_newer = 0;
	struct stat idx_stat, main_stat;
	const char *path = ifp->path;

	ifp->index = xmalloc(strlen(path) + 5);
	sprintf(ifp->index, "%s.idx", path);

	if( fstat(fileno(ifp->fp), &main_stat) ){
		die("stat %s: %s", ifp->path, strerror(errno));
	}

	if( stat(ifp->index, &idx_stat) ){
		if( errno != ENOENT ){
			die("stat %s: %s", ifp->index, strerror(errno));
		}
		/* Index does not exist */
		build_index(ifp);
	} else if(
#if HAVE_STRUCT_STAT_ST_MTIM
		idx_stat.st_mtim.tv_sec <= main_stat.st_mtim.tv_sec
#else
		idx_stat.st_mtimespec.tv_sec <= main_stat.st_mtimespec.tv_sec
#endif
	){
		/* Index is stale; rebuild it */
		build_index(ifp);
	} else {
		/* Index is fresh; read it */
		index_newer = 1;
		FILE *idx = xfopen(ifp->index, "r");
		/* TODO: read some magic numbers, maybe a hash of the input */
		if( fread(&ifp->lines, sizeof ifp->lines, 1, idx) != 1 ){
			die("Failed to read index: %s", strerror(errno));
		}
		ifp->idx = xrealloc(NULL, ifp->lines, sizeof *ifp->idx, NULL);
		if( fread(ifp->idx, sizeof *ifp->idx, ifp->lines, idx)
				!= ifp->lines ){
			die("Failed to read index: %s", strerror(errno));
		}
		if( fclose(idx) ){
			die("close %s: %s\n", ifp->index, strerror(errno));
		}
	}
}

static void
build_index(struct indexed_file *ifp)
{
	size_t capacity;
	size_t count = 0;
	int c;
	size_t i = 0;
	FILE *idx = xfopen(ifp->index, "w");
	ifp->idx = xrealloc(NULL, capacity = 256, sizeof *ifp->idx, NULL);
	ifp->idx[i++] = count;
	while( (c = fgetc(ifp->fp)) != EOF ){
		count += 1;
		if( c == '\n' ){
			ifp->idx[i++] = count;
		}
		if( i == capacity ){
			ifp->idx = xrealloc(ifp->idx, capacity *= 2,
				sizeof *ifp->idx, NULL
			);
		}
	}
	i -= 1;
	ifp->lines = i;
	if( fwrite(&i, sizeof i, 1, idx) != 1 ){
		die("fwrite %s: %s\n", ifp->index, strerror(errno));
	}
	if( fwrite(ifp->idx, sizeof *ifp->idx, i, idx) != i ){
		die("fwrite %s: %s\n", ifp->index, strerror(errno));
	}
	if( fclose(idx) ){
		die("close %s: %s\n", ifp->index, strerror(errno));
	}
}

void
xfseek(struct indexed_file *ifp, long line)
{
	line -= 1;
	if( line < 0 || ifp->lines - line <= 0 ){
		fputs("outside bounds\n", stderr);
		exit(1);
	} else if( fseek(ifp->fp, ifp->idx[line], SEEK_SET) == -1 ){
		perror(ifp->path);
		exit(1);
	}
}
