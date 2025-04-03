

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

bool
isPalindrome(int x)
{
	if(x < 0) {
		return false;
	}
	char digits[64] = {0};
	int i;
	for (i = 0; x; i += 1) {
		digits[i] = x % 10;
		x = x/10;
	}
	i -= 1;
	for ( int j = 0; j < i; j += 1, i -= 1 ) {
		if (digits[i] != digits[j]) {
			return false;
		}
	}
	return true;
}

int
main(int argc, char **argv)
{
	int tests[] = { -1, 0, 101, 1221, 12321, 12331 };
	for (int *t = tests; t < tests + sizeof tests / sizeof *tests; t += 1) {
		printf("%d: %s\n", *t, isPalindrome(*t) ? "true" : "false");
	}
	return 0;
}
