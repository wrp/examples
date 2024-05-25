
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*
Given two sorted arrays nums1 and nums2 of size m and n respectively,
return the median of the two sorted arrays.

The overall run time complexity should be O(log (m+n)).



Example 1:

Input: nums1 = [1,3], nums2 = [2]
Output: 2.00000
Explanation: merged array = [1,2,3] and median is 2.
Example 2:

Input: nums1 = [1,2], nums2 = [3,4]
Output: 2.50000
Explanation: merged array = [1,2,3,4] and median is (2 + 3) / 2 = 2.5.


Constraints:

nums1.length == m
nums2.length == n
0 <= m <= 1000
0 <= n <= 1000
1 <= m + n <= 2000
-106 <= nums1[i], nums2[i] <= 106
*/

#define min(x, y) ((x) < (y) ? (x) : (y))

double
findMedianSortedArrays(int *na, int n, int *ma, int m)
{
	int merged[2048];
	int *mp = merged;
	int *ne = na + n, *me = ma + m;
	while (na < ne && ma < me) {
		if (*na < *ma) {
			*mp++ = *na++;
		} else {
			*mp++ = *ma++;
		}
	}
	while (ma < me) {
		*mp++ = *ma++;
	}
	while (na < ne) {
		*mp++ = *na++;
	}
	size_t len = mp - merged;
	if (len == 0) {
		return 0.0;
	} else if ( len % 2 ){
		return merged[len/2];
	} else {
		return (merged[len/2] + merged[len/2 - 1]) / 2.0;
	}
}


double
f(int *n1, int s, int *n2, int z)
{
	return findMedianSortedArrays(n1, s, n2, z);
}

int
main(int argc, char **argv)
{
	int n[] = {1, 3, 4, 5, 7, 11};
	int m[] = {2, 9};
	printf("median of ");
	for (int i = 0; i < sizeof n / sizeof *n; i += 1) {
		printf("%d, ", n[i]);
	}
	puts(" and");
	for (int i = 0; i < sizeof m / sizeof *m; i += 1) {
		printf("%d, ", m[i]);
	}

	printf (" is: %g\n", f(n, sizeof n / sizeof *n,
		m, sizeof m / sizeof *m));

	return 0;
}
