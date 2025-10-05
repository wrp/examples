
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
/*
time_t is seconds since epoch
struct timeval  == { .tv_sec, .tv_usec }
struct timespec == { .tv_sec, .tv_nsec }

Common macros:
	timersub(&end, &start, &diff);   // timeval
	timerclear(struct timeval *tv) → set to 0
	timerisset(const struct timeval *tv) → non-zero?
	timercmp(const struct timeval *a, const struct timeval *b, CMP) →
		compare using a relational operator (e.g., <, <=, ==, >=, >).
	timeradd(const struct timeval *a, const struct timeval *b,
		struct timeval *res) → res = a + b
		(normalizes tv_usec < 1,000,000).
	timersub(const struct timeval *a, const struct timeval *b,
		struct timeval *res) → res = a − b (normalizes).

	Conversions:
	TIMEVAL_TO_TIMESPEC(const struct timeval *tv, struct timespec *ts)
	TIMESPEC_TO_TIMEVAL(struct timeval *tv, const struct timespec *ts).
	Some systems (OpenBSD) provide timerisvalid(const struct timeval *tv)


	timespecadd(a,b,result);         // timespec
	timespecsub(a,b,result);         // timespec
	timespecisset(a);
	timespecclear(a);

*/

void
print_time(void)
{

	time_t t;
	time(&t);
	char *d = ctime(&t);
	printf("%s", d);  /* value returned has newline */
	assert(d[24] == '\n');
}


static double
span_s(struct timespec a, struct timespec b)
{
	return (b.tv_sec - a.tv_sec) +
		(b.tv_nsec - a.tv_nsec) / 1e9;
}


int
main(void)
{
	struct timespec t0, t1;

	// timespec_get(&t0, TIME_UTC);   // start time
	clock_gettime(CLOCK_MONOTONIC, &t0);

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

	clock_gettime(CLOCK_MONOTONIC, &t1);
	double dur = span_s(t0, t1);  // Seconds from start to finish
	return 0;
}
