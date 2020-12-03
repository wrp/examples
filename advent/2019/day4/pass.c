
/*
--- Day 4: Secure Container ---
You arrive at the Venus fuel depot only to discover it's protected
by a password. The Elves had written the password on a sticky note,
but someone threw it out.

However, they do remember a few key facts about the password:

It is a six-digit number.
The value is within the range given in your puzzle input.
Two adjacent digits are the same (like 22 in 122345).
Going from left to right, the digits never decrease;
they only ever increase or stay the same (like 111123 or 135679).
Other than the range rule, the following are true:

111111 meets these criteria (double 11, never decreases).
223450 does not meet these criteria (decreasing pair of digits 50).
123789 does not meet these criteria (no double).
How many different passwords within the range given in your puzzle input meet these criteria?

An Elf just remembered one more important detail: the two adjacent
matching digits are not part of a larger group of matching digits.

Given this additional criterion, but still ignoring the range rule,
the following are now true:

112233 meets these criteria because the digits never decrease and
all repeated digits are exactly two digits long.  123444 no longer
meets the criteria (the repeated 44 is part of a larger group of
444).  111122 meets the criteria (even though 1 is repeated more
than twice, it still contains a double 22).  How many different
passwords within the range given in your puzzle input meet all of
the criteria?

input: 246540-787419
*/

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

static int
valid(int i)
{
	char buf[64];
	int c;
	int pair = 0;
	int monotonic = 1;
	c = snprintf(buf, sizeof buf, "%d", i);
	if( c >= (int)sizeof buf ){
		errx(1, "Invalid input\n");
	}
	for( int i = 0; i < c - 1; i++ ){
		if( buf[i] == buf[i+1]
			&& (i == 0 || buf[i] != buf[ i - 1])
			&& (i == c-2 || buf[i] != buf[i+2])
		){
			pair = 1;
		}
		if( buf[i] > buf[i+1] ){
			monotonic = 0;
		}
	}
	return pair && monotonic;
}

int
main(int argc, char **argv)
{
	int low = argc > 1 ? strtol(argv[1], NULL, 0) : 246540;
	int high = argc > 2 ? strtol(argv[2], NULL, 0) : 787419;
	int count = 0;
	for( int i = low; i <= high; i++ ) {
		if( valid(i) ) {
			count += 1;
		}
	}
	printf("%d\n", count);
	return 0;
}
