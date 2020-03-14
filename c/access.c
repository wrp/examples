
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "util.h"

/* Open a file.  Pause.  Upon receipt of signal, read from the file.
 * Testing how access time is modified
 */

sig_atomic_t usr1, usr2, hup;

void
handle(int sig, siginfo_t *i, void *v)
{
	(void)i;
	(void)v;
	switch(sig) {
	case SIGUSR1: usr1=1; break;
	case SIGUSR2: usr2=1; break;
	case SIGHUP: hup=1; break;
	}
	return;
}

int
main(int argc, char **argv)
{
	struct sigaction act;
	const char *path = argc > 1 ? argv[1] : "foo";
	FILE *fp;

	fp = xfopen(path, "a+");

	printf("pid: %d\n", getpid());

	memset(&act, 0, sizeof act);
	act.sa_sigaction = handle;
	if(sigaction( SIGUSR1, &act, NULL )) { perror("sigaction"); exit(1); }
	if(sigaction( SIGUSR2, &act, NULL )) { perror("sigaction"); exit(1); }
	if(sigaction( SIGHUP, &act, NULL )) { perror("sigaction"); exit(1); }

	while(1) {
		const sigset_t mask = {0};
		sigsuspend(&mask);
		if(usr1 || usr2) {
			if(fseek(fp, 0L, SEEK_SET)) {
				perror("fseek");
			}
			puts("seek");
		}
		if(usr2) {
			char b;
			if(fread(&b, 1, 1, fp) != 1) perror("fread");
			puts("fread");
		}
		if(hup) {
			if(fwrite("f", 1, 1, fp) != 1) perror("fwrite");
			if( fflush(fp)) perror("flush");
			puts("fwrite");
		}
		usr1 = usr2 = hup = 0;
	}
	return 0;
}
