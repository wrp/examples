
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>

void
print_time(void)
{

	time_t t;
	time(&t);
	char *d = ctime(&t);
	printf("%s", d);  /* value returned has newline */
	assert(d[24] == '\n');
}

int
main(void)
{
	struct timeval start;
	print_time();

	int s = gettimeofday(&start, NULL);
	if (s == 0) {
		struct timeval end, diff;

		if( 0 == gettimeofday(&end, NULL)) {
			timersub(&end, &start, &diff);
			printf("Completed in %ld.%06lds\n",
				(long)diff.tv_sec, (long)diff.tv_usec);
		}
	}
	return 0;
}
