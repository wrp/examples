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
	int *idx;
	size_t lines;  /* Number of lines in file */
};

void build_index(struct indexed_file *ifp);
void open_indexed(const char *path, struct indexed_file *ifp);

int
main(int argc, char **argv)
{
	struct indexed_file ifp[1];

	if( argc < 2 ) {
		char *base = strrchr(argv[0], '/');
		base = base ? base + 1 : argv[0];
		printf("usage: %s input-file [line-num ...]\n", base);
		return EXIT_SUCCESS;
	}
	open_indexed(argv[1], ifp);
	for( argv += 2; *argv; argv++ ) {
		size_t line = strtol(*argv, NULL, 10);
		int c;
		if(line > 0 && line - 1 < ifp->lines) {
			fseek(ifp->fp, ifp->idx[line - 1], SEEK_SET);
			while( ( c = fgetc(ifp->fp)) != EOF && c != '\n' ) {
				putchar(c);
			}
			putchar('\n');
		} else {
			fputs("outside bounds\n", stderr);
		}
	}
}

void
open_indexed(const char *path, struct indexed_file *ifp)
{
	ifp->fp = xfopen(path, "r");
	ifp->path = path;
	build_index(ifp);
}

void
build_index(struct indexed_file *ifp)
{
	FILE *idx;
	int c;
	char *idx_path;
	int index_newer = 0;
	struct stat idx_stat, main_stat;
	const char *path = ifp->path;
	size_t capacity;
	size_t count = 0;
	size_t i = 0;

	idx_path = xmalloc(strlen(path) + 5);
	sprintf(idx_path, "%s.idx", path);

	if( fstat(fileno(ifp->fp), &main_stat) ) {
		die("stat %s: %s", ifp->path, strerror(errno));
	}

	if( stat(idx_path, &idx_stat) ) {
		if( errno != ENOENT ) {
			die("stat %s: %s", idx_path, strerror(errno));
		}
	} else if(
#if HAVE_STRUCT_STAT_ST_MTIM
		idx_stat.st_mtim.tv_sec > main_stat.st_mtim.tv_sec
#else
		idx_stat.st_mtimespec.tv_sec > main_stat.st_mtimespec.tv_sec
#endif
			) {
		/* Index is newer, no need to rebuild */
		index_newer = 1;
		idx = xfopen(idx_path, "r");
		/* TODO: read some magic numbers, maybe a hash of the input file */
		if( fread(&ifp->lines, sizeof ifp->lines, 1, idx) != 1 ) {
			die("Failed to read index: %s", strerror(errno));
		}
		ifp->idx = xrealloc(NULL, ifp->lines, sizeof *ifp->idx, NULL);
		if( fread(ifp->idx, sizeof *ifp->idx, ifp->lines, idx) != ifp->lines ) {
			die("Failed to read index: %s", strerror(errno));
		}
		goto end;
	}
	/* Index does not exist or is stale: rebuild */
	idx = xfopen(idx_path, "w");
	ifp->idx = realloc(NULL, capacity = 256);
	ifp->idx[i++] = count;
	while( ( c = fgetc(ifp->fp)) != EOF ) {
		count += 1;
		if( c == '\n' )
			ifp->idx[i++] = count;
		if( i == capacity ) {
			ifp->idx = xrealloc(ifp->idx, capacity *= 2, sizeof *ifp->idx, NULL);
		}
	}
	i -= 1;
	ifp->lines = i;
	if( fwrite(&i, sizeof i, 1, idx) != 1 ) {
		die("fwrite %s: %s\n", idx_path, strerror(errno));
	}
	if( fwrite(ifp->idx, sizeof *ifp->idx, i, idx) != i ) {
		die("fwrite %s: %s\n", idx_path, strerror(errno));
	}
end:
	if( fclose(idx)) {
		die("close %s: %s\n", idx_path, strerror(errno));
	}
}
