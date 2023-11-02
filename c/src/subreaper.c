
/* Demonstrate setting a subreaper which becomes parent of orphans */
/* Linux only. See prctl(2) */

#include <err.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <unistd.h>

void xclose(int fd);
void xpipe(int *fd);

void
show_pid(const char *msg, int index, pid_t p)
{
	printf("%s(%d) -> %" PRIdMAX "\n", msg, index, (intmax_t)p);
}

void
child(const char *msg, pid_t initial_parent)
{
	show_pid(msg, 1, initial_parent);
	int i = 2;
	while( getppid() == initial_parent ){
		i += 1;
	}
	show_pid(msg, i, getppid());
}

int
spawn_one(const char *msg)
{

	pid_t mypid = getpid();
	switch( fork() ){
	case -1:
		err(1, "fork");
	default:
		exit(0);
	case 0:
		child(msg, mypid);
	}
	return 0;
}


int main(void)
{
	int status;
	show_pid("parent pid", 1, getpid());
	spawn_one("first");
	if( prctl(PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0) == -1 ){
		err(1, "prctl");
	}
	show_pid("subreaper", 2, getpid());
	switch(fork()){
	case -1: err(1, "fork");
	case 0:
		spawn_one("second");
		exit(1);
	default:
	}
	wait(&status);
	return 0;
}

void
xclose(int fd)
{
	if( close(fd) ){
		err(1, "close(%d)", fd);
	}
}


void
xpipe(int *fd)
{
	if( pipe(fd) ){
		err(1, "pipe");
	}
}
