

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

/*
Given a string s, return the longest palindromic substring in s.

Example 1:

Input: s = "babad"
Output: "bab"
Explanation: "aba" is also a valid answer.
Example 2:

Input: s = "cbbd"
Output: "bb"

Constraints:

1 <= s.length <= 1000
s consist of only digits and English letters.
*/

/* solution is an implementation of Manacher's algorithm */

struct string {
	const char *data;
	size_t len;
};


// Manacher's algorithm uses an auxiliary string with a unique
// character inserted between each character of the given string
// to ensure an odd length.  eg, instead of looking at "abc" or "abcd",
// we consider "|a|b|c|" and "|a|b|c|d|"
int
getc_pad(size_t i, const struct string *s)
{
	if (i > 2 * s->len) {
		return EOF;
	}
	if( i % 2 ) {
		return s->data[i/2];
	} else {
		return '|';
	}
}

unsigned
find_longest_centered_at(const struct string s, unsigned center)
{
	unsigned r = 1;
	assert(center < 2 * s.len + 1);
	for (; r < 2 * s.len + 1 - center && r <= center; r += 1) {
		if (getc_pad(center + r, &s) != getc_pad(center - r, &s)) {
			break;
		}
	}
	return r - 1;
}


struct string
manacher(const struct string s)
{
	struct string rv = {0};
	unsigned *radii;
	unsigned center = 0;
	unsigned radius = 0;
	unsigned max_radius = 0;
	unsigned max_center;

	radii = calloc(s.len * 2 + 1, sizeof *radii);
	while (center < s.len * 2 + 1) {
		radius = radii[center] = find_longest_centered_at(s, center);
		if (radius > max_radius) {
			max_radius = radius;
			max_center = center;
		}

		assert(radius <= center);

		unsigned old_radius = radius;
		unsigned old_center = center;
		radius = 0;
		center += 1;

		while (center <= old_center + old_radius) {
			assert(2 * old_center >= center);
			unsigned mirrored_center = 2 * old_center - center;
			unsigned max_mirrored_radius = old_center + old_radius - center;
			if (radii[mirrored_center] < max_mirrored_radius) {
				radii[center] = radii[mirrored_center];
			} else if (radii[mirrored_center] > max_mirrored_radius) {
				radii[center] = max_mirrored_radius;
			} else {
				radius = max_mirrored_radius;
				break;
			}
			center += 1;
		}
	}

	rv.len = max_radius;
	rv.data = s.data + (max_center - max_radius) / 2;
	return rv;
}

char *
longestPalindrome(char *a)
{
	struct string s = {a, strlen(a)};
	struct string t = manacher(s);
	char *rv = strdup(t.data);
	rv[t.len] = 0;
	return rv;
}

static void
test_case_v(const char *str, const struct string expect)
{
	struct string s = {str, strlen(str)};
	struct string t = manacher(s);
	if (t.len != expect.len || t.data != expect.data) {
		fprintf(stderr, "Test failure: input %s returned ", str);
		fwrite(t.data, 1, t.len, stderr);
		fputc('\n', stderr);
	}
}

static size_t fail_count = 0;

static void
test_case(const char *str, const char *expect)
{
	struct string s = {str, strlen(str)};
	struct string t = manacher(s);
	for (int i = 0; i < t.len; i += 1, expect += 1)  {
		if (t.data[i] != *expect) {
			break;
		}
	}
	if (*expect) {
		fprintf(stderr, "Test failure: input %s returned ", str);
		fwrite(t.data, 1, t.len, stderr);
		fputc('\n', stderr);
		fail_count += 1;
	}
}


static void
start_timer(struct timeval *s)
{
	if (gettimeofday(s, NULL)) {
		perror("gettimeofday");
		exit(1);
	}
}


static void
report_time(const struct timeval *s)
{
	struct timeval end, diff;
	if( gettimeofday(&end, NULL)) {
		perror("gettimeofday");
		return;
	}

	timersub(&end, s, &diff);
	printf(
		"Elapsed time: %ld.%06lds\n",
		(long)diff.tv_sec,
		(long)diff.tv_usec
	);
	return;
}

static void
replace_and_validate(char *str, size_t len, size_t i)
{
		char orig = str[i];
		char expect[len + 1];
		char *base = expect + i + 1;
		expect[len - 1 - (base - expect)] = '\0';
		str[i] = 'z';
		test_case(str, base);
		expect[len - 1 - (base - expect)] = str[i] = orig;
		free(expect);
}


static void
test_repl(const char *palindrome, size_t len)
{
	char src[len];
	memcpy(src, palindrome, len);
	len -= 1;  // len includes trailing '\0'
	for (int i = 0; i < len / 2 - 2; i += 1) {
		char orig = src[i];
		struct string expect = { src + i + 1, len - 2 * (i + 1) };
		src[i] = 'z';
		test_case_v(src, expect);
		src[i] = orig;
	}
}



int
main(int argc, char **argv)
{
	unsigned long count;
	char *end;
	if (argc > 1 && (count = strtoul(argv[1], &end, 10)) > 0 && *end == '\0') {
		argv += 1;
	} else {
		count = 1;
	}
	while (*++argv) {
		printf("%s: %s\n", *argv, longestPalindrome(*argv));
	}
	struct timeval start;
	start_timer(&start);
	for( ; count; count -= 1) {
	test_case("", "");
	test_case("a", "a");
	test_case(" a ", " a ");
	test_case("foo", "oo");
	test_case("fff", "fff");
	test_case("banana", "anana");
	test_case("bananz", "ana");
	test_repl("abcdefghijklmlkjihgfedcba", 26);
	}
	if (fail_count > 0) {
		fprintf(stderr, "%zu tests failed\n", fail_count);
	}
	report_time(&start);
	return fail_count > 0;
}

