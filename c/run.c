#include <assert.h>
#include <errno.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#if 0

The following simple shell script gets munged by bash, and
somehow 4 new panes are created instead of 2.
This code implements a version of it, so I do not have to
install dash everywhere.  And I added a few bells and
whistles.  (error pane does not spawn unless there is some
data on stderr, microsecond precision on the timestamps, etc.)

stdout="$HOME/.bash-run-output/$pid-stdout"
stderr="$HOME/.bash-run-output/$pid-stderr"
hdr="************************************"
{
	printf "\n%s\n%s\n$" "$hdr" "$(date)"
	printf " '%s'" "$@"
	printf "\n"
} \
| tee -a "$stdout" "$stderr" > /dev/null
{
"$@" 2>&3 \
| perl -pe '$|=1; print localtime . ": "' \
| tee -a "$stdout" \
| tmux split-window -l 10 -dI
} 3>&1 \
| perl -pe '$|=1; print localtime . ": "' \
| tee -a "$stderr" \
| tmux split-window -l 10 -dI
#endif

void xpipe(int *fd);
void xclose(int fd);
void xdup2(int dest, int src);
FILE *xfopen(const char *path, const char *mode);
FILE *xfdopen(int, const char *mode);
enum fork_type { parent, child } xfork(void);
void print_args(FILE* f, char **argv);


void
do_split(FILE *out, int is_err, char **argv)
{
	/* Read stdn, prepend timestamp and write to out and a split pane*/
	FILE *split;
	int c;
	int fd[2];
	int first = 1;
	char *tmux_args[] = { "tmux", "split-window", "-p", "50", "-dI", NULL };

	if(is_err) {
		/* Do not spawn a pane until there is some data to show */
		if( (c = getchar()) == EOF) {
			return;
		}
		ungetc(c, stdin);
		tmux_args[2] = "-l";
		tmux_args[3] = "10";
	}

	xpipe(fd);
	switch(xfork()) {
	case child:
		xdup2(fd[0], STDIN_FILENO);
		xclose(fd[0]);
		xclose(fd[1]);
		execvp(tmux_args[0], tmux_args);
		perror("execlp tmux");
		exit(EXIT_FAILURE);
	case parent:
		;
	}

	xclose(fd[0]);

	split = xfdopen(fd[1], "w");
	if(is_err) {
		setbuf(split, NULL);
	}
	fprintf(split, "%s of: ", is_err ? "errors" : "output");
	print_args(split, argv);

	while( (c = getchar()) != EOF ) {
		if(first) {
			char timestamp[128];
			struct timeval tp;
			if( gettimeofday(&tp, NULL) == -1 ) {
				tp.tv_sec = 0;
				tp.tv_usec = 0;
			}
			fflush(out);
			fflush(split);
			fprintf(out, "%6s", is_err ? "error " : "");
			sprintf(timestamp, "%ld.%ld: ", (long)tp.tv_sec, (long)tp.tv_usec);
			fputs(timestamp, out);
			fputs(timestamp, split);
		}
		fputc(c, out);
		fputc(c, split);
		first = (c == '\n');
	}
	fflush(out);
	fflush(split);
	exit(EXIT_SUCCESS);
}

FILE *
check_env(char **argv)
{
	char log_path[PATH_MAX];
	char *base;
	FILE *out;
	char *date;
	struct timeval tp;
	if( getenv("TMUX") == NULL ) {
		fputs("Must be run in tmux\n", stderr);
		exit(EXIT_FAILURE);
	}
	base = getenv("RUN_LOG_DIR");
	if( base == NULL ) {
		base = getenv("HOME");
		if( base == NULL ) {
			fputs("HOME is not set\n", stderr);
			exit(EXIT_FAILURE);
		}
		snprintf(log_path, PATH_MAX, "%s/.run-output/result-%d", base, getppid());
	} else {
		snprintf(log_path, PATH_MAX, "%s/result-%d", base, getppid());
	}
	out = xfopen(log_path, "a");
	fputs("********************************\n", out);
	if (gettimeofday(&tp, NULL) == -1) {
		perror("gettimeofday");
		exit(1);
	}
	date = ctime(&tp.tv_sec);
	assert(date[24] == '\n');
	date[24] = '\0';
	fprintf(out, "%s: ", date);
	print_args(out, argv);
	fflush(out);
	return out;
}


void
do_kid(int *p1, int *p2, int is_err, char **argv, FILE *log_file)
{
	if(xfork() == child) {
		xdup2(p1[0], STDIN_FILENO);
		xclose(p1[0]);
		xclose(p1[1]);
		xclose(p2[0]);
		xclose(p2[1]);
		do_split(log_file, is_err, argv);
		exit(EXIT_SUCCESS);
	}
}

int
main(int argc, char **argv)
{
	int outp[2];
	int errp[2];
	FILE *log_file;
	if(argc == 1) {
		printf("usage: %s shell-command\n\n", basename(argv[0]));
		puts("Run a command with output streams in 2 new tmux panes,");
		puts("with those streams dup'd to files.");
		return 0;
	}
	log_file = check_env(argv + 1);
	xpipe(outp);
	xpipe(errp);
	do_kid(errp, outp, 1, argv + 1, log_file);
	do_kid(outp, errp, 0, argv + 1, log_file);
	xdup2(outp[1], STDOUT_FILENO);
	xdup2(errp[1], STDERR_FILENO);
	xclose(outp[0]);
	xclose(outp[1]);
	xclose(errp[0]);
	xclose(errp[1]);
	/* Double fork so the called process is not a parent */
	if(xfork() == parent) {
		return EXIT_SUCCESS;
	}
	if(xfork() == parent) {
		return EXIT_SUCCESS;
	}
	execvp(argv[1], argv + 1);
	perror("execvp");
	return EXIT_FAILURE;
}

void
xdup2(int dest, int src)
{
	if( dup2(dest, src) == -1 ) {
		perror("dup2");
		exit(EXIT_FAILURE);
	}
}

void
xpipe(int *fd)
{
	if( pipe(fd) != 0 ) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

void
xclose(int fd)
{
	if( close(fd) == -1 ) {
		perror("close");
		exit(EXIT_FAILURE);
	}
}

FILE *
xfopen(const char *path, const char *mode)
{
	FILE *fp = strcmp(path, "-") ? fopen(path, mode) :
		*mode == 'r' ? stdin : stdout;
	if( fp == NULL ) {
		perror(path);
		exit(EXIT_FAILURE);
	}
	return fp;
}


FILE *
xfdopen(int fd, const char *mode)
{
	FILE *fp = fdopen(fd, mode);
	if( fp == NULL ) {
		perror("fdopen");
		exit(EXIT_FAILURE);
	}
	return fp;
}


enum fork_type
xfork(void)
{
	switch(fork()) {
	case -1:
		perror("fork");
		exit(EXIT_FAILURE);
	case 0:
		return child;
	default:
		return parent;
	}
}


void
print_args(FILE* f, char **argv)
{
	fprintf(f, "%s", *argv++);
	while( *argv ) {
		fprintf(f, " '%s'", *argv++);
	}
	fputc('\n', f);
}
