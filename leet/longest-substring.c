
/*
Given a string s, find the length of the longest substring without repeating characters.

Example 1:

Input: s = "abcabcbb"
Output: 3
Explanation: The answer is "abc", with the length of 3.
Example 2:

Input: s = "bbbbb"
Output: 1
Explanation: The answer is "b", with the length of 1.
Example 3:

Input: s = "pwwkew"
Output: 3
Explanation: The answer is "wke", with the length of 3.
Notice that the answer must be a substring, "pwke" is a subsequence and not a substring.


Constraints:

0 <= s.length <= 5 * 104
s consists of English letters, digits, symbols and spaces.
*/

#define NDEBUG 1
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int
lengthOfLongestSubstring(char *s)
{
	int max = 0;
	int count = 0;
	char *start_of_substring = s;
	unsigned lut[256] = {0};

	for (; *s; s += 1) {
		if (lut[*s] == 0) {
			count += 1;
			lut[*s] += 1;
		} else {
			if (count > max) {
				max = count;
			}
			do {
				assert(lut[*start_of_substring] == 1);
				lut[*start_of_substring++] = 0;
				count -= 1;
			} while(start_of_substring[-1] != *s);
			assert(start_of_substring[-1] == *s);
			assert(start_of_substring <= s);
			count += 1;
			assert(lut[start_of_substring[-1]] == 0);
			lut[start_of_substring[-1]] = 1;
		}
	}
	return count > max ? count : max;
}

int
main(int argc, char **argv)
{
	assert(lengthOfLongestSubstring("b") == 1);
	assert(lengthOfLongestSubstring("abcabcbb") == 3);
	assert(lengthOfLongestSubstring("bbbbbbbb") == 1);
	assert(lengthOfLongestSubstring("pwwkew") == 3);
	assert(lengthOfLongestSubstring("abcdefghijklmabcdk") == 13);
	return 0;
}
