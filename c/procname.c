/* Show process name

purloined from: https://stackoverflow.com/questions/31500821
*/

#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <sys/proc_info.h>

extern int proc_pidinfo(int pid, int flavor, uint64_t arg, user_addr_t buffer,
	uint32_t  buffersize);
#define SHOW_ZOMBIES 0

int
main(int argc, char **argv)
{
	if( argc < 2 ) {
		printf("Usage: %s <pid> [pid ...]\n", basename(argv[0]));
		return 0;
	}

	struct proc_taskallinfo info;

	for( argv += 1 ; *argv; argv++ ) {
		int pid = strtol(*argv, NULL, 10);
		int ret = proc_pidinfo(pid,
			PROC_PIDTASKALLINFO, SHOW_ZOMBIES,
			(user_addr_t) &info, sizeof info
		);
		printf("ret=%d, %s result=%s\n", ret,
			*argv,
			(char *) info.pbsd.pbi_comm);
	}

	return 0;
}
