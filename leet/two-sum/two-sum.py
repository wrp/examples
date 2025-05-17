#!/usr/bin/env python

import sys
import platform
from typing import List

if sys.version_info.major != 3:
    raise SystemExit(f'Unexpected python version {platform.python_version()}: must use python 3')

# sample input
# target	answer	array
# 9	0,3	[2, 14, 18, 7, -3, 1]
# 9	1,3	[14, 2, 18, 7, -3, 1]
# 9	3,5	[14, 28, 18, 7, -3, 2]
# 16	0,1	[2, 14, 18, 7, -3, 1]
# 20	0,2	[2, 14, 18, 7, -3, 1]
# 4	3,4	[2, 14, 18, 7, -3, 1]

def find_sum(values: List[int], target: int):
    s = sorted(list(enumerate(values)), key = lambda x: x[1])
    left = 0
    right = len(s) - 1
    while left < right:
        if s[left][1] + s[right][1] == target:
            a, b = s[left][0], s[right][0]
            if a > b:
                a, b = b, a
            return a, b

        if s[left][1] + s[right][1] < target:
            left += 1
        else:
            right -= 1

    return None

class test_case:
    def __init__(self, line):
        target, answer, array_text = line.split('\t')
        self.target = int(target)
        self.answer = list(map(int, answer.split(',')))
        exec('self.array = ' + array_text.strip())

    def __repr__(self):
        return f'{self.array} -> {self.answer} = {self.target}'


count = 0
for line in sys.stdin:
    if not line[0].isdigit():
        continue

    t = test_case(line)
    result = find_sum(t.array, t.target)
    if result and result[0] == t.answer[0] and result[1] == t.answer[1]:
        count += 1
    else:
        print(f'test failed: {t}', file=sys.stderr)

print(f'{count} tests passed')
