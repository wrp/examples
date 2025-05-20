#!/usr/bin/env python

from collections import namedtuple
from io import StringIO
import sys


class Solution:
    def lengthOfLongestSubstring(self, s: str) -> int:
        start = i = 0
        lut = {}
        max_ = 0
        while i < len(s):
            if lut.get(s[i]) is None:
                lut[s[i]] = 1
            else:
                if i - start > max_:
                    max_ = i - start
                while s[start] != s[i]:
                    del lut[s[start]]
                    start += 1
                start += 1
            i += 1
        if i - start > max_:
            max_ = i - start

        return max_

tests = '''result	string
1	b
3	abcabcbb
1	bbbbbbbb
4	bbbbabcd
3	pwwkew
13	abcdefghijklmabcdk
'''
input = StringIO(tests)
record = namedtuple('record', input.readline().strip().split('\t'))
count = 0
for line in input:
    x = record(*line.strip().split('\t'))
    res = Solution().lengthOfLongestSubstring(x.string)
    if res != int(x.result):
        print(f'Test failed for {x.string}.  Got {res}, expected {x.result}', file=sys.stderr)
    else:
        count += 1

print(f'{count} tests passed')



