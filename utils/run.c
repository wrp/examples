/*
   Copyright 2020 William R. Pursell

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <assert.h>
#include <errno.h>
#include <libgen.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "config.h"
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


TODO: stop using tmux.  Perhaps pull code out of dvtm and make
this completely stand alone.
#endif

void xpipe(int *fd);
void xclose(int fd);
void xdup2(int dest, int src);
FILE *xfopen(const char *path, const char *mode);
FILE *xfdopen(int, const char *mode);
enum fork_type { parent, child } xfork(pid_t *);
void print_args(FILE* f, char **argv);
void tee_stdin(FILE *out, FILE *split, const char *msg);
FILE * get_alternate_pane(void);


void
do_split(FILE *out, int is_err, char **argv)
{
	/* Read stdn, prepend timestamp and write to out and a split pane*/
	FILE *split;

	if(is_err) {
		int c;
		/* Do not spawn a pane until there is some data to show */
		if( (c = getchar()) == EOF) {
			return;
		}
		ungetc(c, stdin);
	}

	split = get_alternate_pane();

	if(is_err) {
		setbuf(split, NULL);
	}
	fprintf(split, "%s of: ", is_err ? "errors" : "output");
	print_args(split, argv);

	tee_stdin(out, split, is_err ? "error" : "");
	exit(EXIT_SUCCESS);
}

FILE *
check_env(char **argv)
{
	char out_path[PATH_MAX];
	char *base;
	FILE *out;
	char *date;
	struct timeval tp;
	if( getenv("TMUX") == NULL && getenv("DVTM_CMD_FIFO") == NULL) {
		fputs("Must be run in tmux or dvtm with a cmd fifo\n", stderr);
		exit(EXIT_FAILURE);
	}
	base = getenv("RUN_OUTPUT_FMT");
	if( base == NULL ) {
		base = getenv("HOME");
		if( base == NULL ) {
			fputs("HOME is not set\n", stderr);
			exit(EXIT_FAILURE);
		}
		snprintf(out_path, PATH_MAX, "%s/.run-output/result-%d", base, getppid());
	} else {
		size_t i = 0;
		for( ; i < PATH_MAX - 1 && *base; i++ ) {
			if( *base == '%' && base[1] == 'p' ) {
				i += snprintf(out_path + i, PATH_MAX - i, "%d", getppid());
				base += 2;
			} else {
				out_path[i] = *base++;
			}
		}
		out_path[i] = '\0';
	}
	out = xfopen(out_path, "a");
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
do_kid(int *p1, int *p2, int is_err, char **argv, FILE *out_file)
{
	if(xfork(NULL) == child) {
		xdup2(p1[0], STDIN_FILENO);
		xclose(p1[0]);
		xclose(p1[1]);
		xclose(p2[0]);
		xclose(p2[1]);
		do_split(out_file, is_err, argv);
		exit(EXIT_SUCCESS);
	}
}


static void
ignore(int sig)
{
	struct sigaction act;
	memset(&act, 0, sizeof act);
	act.sa_handler = SIG_IGN;
	if(sigaction( sig, &act, NULL )) {
		perror("sigaction");
		exit(1);
	}
}


int
main(int argc, char **argv)
{
	int outp[2];
	int errp[2];
	int status;
	pid_t p;
	FILE *out_file;
	ignore(SIGINT);
	if(argc == 1) {
		printf("usage: %s shell-command\n\n", basename(argv[0]));
		puts("version: " VERSION);
		puts("Run a command with output streams in 2 new panes,");
		puts("with those streams dup'd to a file.  The pane for stderr");
		puts("is deferred, and will not appear if no errors are written.");
		puts("Both streams are also written to the file specified in");
		puts("$RUN_OUTPUT_FMT[$HOME/.run-output/results-%p].");
		puts("Occurrences of '%p' in RUN_OUTPUT_FMT will be replaced with");
		puts("the caller's pid.");
		puts("Note that %% is not an escape sequence, and there is no way");
		puts("to create output files with '%p' in the name.");

		return 0;
	}
	out_file = check_env(argv + 1);
	xpipe(outp);
	xpipe(errp);
	do_kid(errp, outp, 1, argv + 1, out_file);
	do_kid(outp, errp, 0, argv + 1, out_file);
	/* fork so the called process is not the parent of the helper */
	switch( xfork(&p)) {
	case child:
		xdup2(outp[1], STDOUT_FILENO);
		xdup2(errp[1], STDERR_FILENO);
		xclose(outp[0]); xclose(outp[1]); xclose(errp[0]); xclose(errp[1]);
		execvp(argv[1], argv + 1);
		perror("execvp");
		status = EXIT_FAILURE;
	case parent:
		xclose(outp[0]); xclose(outp[1]); xclose(errp[0]); xclose(errp[1]);
		waitpid(p, &status, 0);
		if( WIFSIGNALED(status)) {
			fprintf(stderr, "Killed by signal %d\n", WTERMSIG(status));
		} else if( WIFEXITED(status)) {
			fprintf(stderr, "Exited with status %d\n", WEXITSTATUS(status));
		}
	}
	return status;
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
xfork(pid_t *p)
{
	pid_t pid;
	enum fork_type r = parent;
	switch( pid = fork() ) {
	case -1:
		perror("fork");
		exit(EXIT_FAILURE);
	case 0:
		r = child;
		/* Fall thru */
	default:
		if( p ) {
			*p = pid;
		}
	}
	return r;
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


static void
write_timestamp(FILE *out, FILE *split, const char *msg)
{
	char timestamp[128];
	struct timeval tp;
	if( gettimeofday(&tp, NULL) == -1 ) {
		tp.tv_sec = 0;
		tp.tv_usec = 0;
	}
	fflush(out);
	fflush(split);
	sprintf(timestamp, "%10ld.%06ld: ", (long)tp.tv_sec, (long)tp.tv_usec);
	fprintf(out, "%-7s", msg);
	fputs(timestamp, out);
	fputs(timestamp, split);
}

void
tee_stdin(FILE *out, FILE *split, const char *msg)
{
	int first = 1;
	int c;
	while( (c = getchar()) != EOF ) {
		if(first) {
			write_timestamp(out, split, msg);
		}
		fputc(c, out);
		fputc(c, split);
		first = (c == '\n');
	}
	fflush(out);
	fflush(split);
}

/* Create path of fifo we will use to get data to cat.  To
 * be removed on exit.
 */
char *
create_fifo_path(char *path, size_t s)
{
	char name[NAME_MAX];
	if(getcwd(path, s) == NULL) {
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
	sprintf(name, "/run-dvtm-fifo.%d", getpid());
	strncat(path, name, s - strlen(path));
	return path;
}

void
delete_fifo(void)
{
	char path[PATH_MAX];
	unlink(create_fifo_path(path, sizeof path));
}

/*
 * Return a FILE * into which data should be written (this will display
 * in an alternate pane.
 */
FILE *
get_alternate_pane(void)
{
	char *tmux_args[] = { "tmux", "split-window", "-p", "50", "-dI", NULL };
	int fd[2];
	char *fifo;

	if( (fifo = getenv("DVTM_CMD_FIFO")) != NULL) {
		char path[PATH_MAX];
		FILE *cmd_fifo;
		atexit(delete_fifo);
		if(mkfifo(create_fifo_path(path, sizeof path), 0700) == -1) {
			perror("mkfifo");
			exit(EXIT_FAILURE);
		}
		cmd_fifo = xfopen(fifo, "a");
		fprintf(cmd_fifo, "create \"cat < %s; while read; do :; done\"\n", path);
		fclose(cmd_fifo);
		return xfopen(path, "a");
	} else {
		xpipe(fd);
		switch(xfork(NULL)) {
		case child:
			xdup2(fd[0], STDIN_FILENO);
			xclose(fd[0]);
			xclose(fd[1]);
			execvp(tmux_args[0], tmux_args);
			perror("execlp tmux");
			exit(EXIT_FAILURE);
		case parent:
			xclose(fd[0]);
			return xfdopen(fd[1], "w");
		}
	}
}
