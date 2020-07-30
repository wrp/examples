/* Show process name

purloined from: https://stackoverflow.com/questions/31500821
and
https://chromium.googlesource.com/external/github.com/giampaolo/
	psutil/+/refs/heads/master/psutil/arch/osx/process_info.c#166
*/

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <sys/proc_info.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <err.h>
#include "xutil.h"

extern int proc_pidinfo(int pid, int flavor, uint64_t arg, user_addr_t buffer,
	uint32_t  buffersize);
#define SHOW_ZOMBIES 0

static void
showinfo(int pid)
{
	int mib[3] = { CTL_KERN, KERN_ARGMAX, 0 };
	int argmaxi;
	int argc;
	size_t argmax;
	char *procargs, *t;

	size_t size = sizeof argmaxi;
	struct proc_taskallinfo info;
	user_addr_t v = (user_addr_t)&info;
	proc_pidinfo(pid, PROC_PIDTASKALLINFO, SHOW_ZOMBIES, v, sizeof info);
	printf("pid=%d cmd=%s ", pid, (char *) info.pbsd.pbi_comm);

	if( sysctl(mib, 2, &argmaxi, &size, NULL, 0)) {
		err(EXIT_FAILURE, "sysctl argmax");
	}
	procargs = xmalloc(argmax = argmaxi);
	mib[1] = KERN_PROCARGS2;
	mib[2] = pid;
	if( sysctl(mib, 3, procargs, &argmax, NULL, 0) < 0 ) {
		err(EXIT_FAILURE, "sysctl args");
	}
	memcpy(&argc, procargs, sizeof argc);
	printf("argc=%d", argc);
	int i = 0;
	for( t = procargs + sizeof argc; *t == '\0'; t++ ) {
		;
	}
	int flag = 1;
	assert(*t);
	while( i < argc + 1 ) {
		if(flag) {
			printf("\n\t%d:\t", i + 1);
			flag = 0;
		}
		putchar(*t);
		if( ! *t ) {
			i++;
			flag = 1;
			while(*++t==0)
				;
			t -= 1;
		}
		t += 1;
	}
	putchar('\n');
}

int
main(int argc, char **argv)
{
	if( argc < 2 ) {
		printf("Usage: %s <pid> [pid ...]\n", basename(argv[0]));
		return 0;
	}


	for( argv += 1 ; *argv; argv++ ) {
		int pid = strtol(*argv, NULL, 10);
		showinfo(pid);
	}

	return 0;
}
