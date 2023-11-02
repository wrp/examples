#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	int flag = AT_SYMLINK_FOLLOW;
	int fd = 0;
	const char *dir = ".";
	if( argc < 2 || !strcmp(argv[1], "-?") ){
		printf("usage: %s [-h] [-d dir] src [target ...]",
			basename(argv[0]));
		return 0;
	}
	if( !strcmp(argv[1], "-h") ){
		flag = 0;
		argv += 1;
		argc -= 1;
	}
	if( !strcmp(argv[1], "-d") ){
		dir = argv[2];
		if( dir == NULL ){
			fputs("No argument given for -d", stderr);
			return 1;
		}
		argv += 2;
		argc -= 2;
	}
	if( !strcmp(argv[1], "--") ){
		argv += 1;
		argc -= 1;
	}
	if( argc < 3 ){
		fputs("No targets specified -? for usage", stderr);
		return 1;
	}
	char *src = *++argv;

	if( ! flag && ! fd ){
		fd = open(dir, 0);
		if( fd == -1 ){
			perror(dir);
		}
	}
	for( char *dst = *++argv; dst; dst = *++argv ){
		char b[512];
		snprintf(b, sizeof b, "%s->%s", dst, src);
		if( fd ){
			if( linkat(fd, src, fd, dst, flag) ){
				perror(b);
				return 1;
			}
		} else {
			if( link(src, dst) ){
				perror(b);
				return 1;
			}
		}
	}
	return 0;
}
