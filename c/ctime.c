#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	struct timeval tp;
	char *date;
	if (gettimeofday(&tp, NULL) == -1) {
		perror("gettimeofday");
		exit(1);
	}
	date = ctime(&tp.tv_sec);
	printf("%lu == %s", tp.tv_sec, date);
}
