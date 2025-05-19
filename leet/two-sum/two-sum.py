#!/usr/bin/env python

import heapq
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

class entry:
    def __init__(self, value, idx, reverse=False):
        self.idx = idx
        self.value = value
        self.reverse = reverse

    def __lt__(self, other):
        if self.reverse:
            if self.value == other.value:
                return other.idx < self.idx
            return other.value < self.value
        else:
            if self.value == other.value:
                return self.idx < other.idx
            else:
                return self.value < other.value

    def __repr__(self):
        return f'({self.value}, {self.idx})'

def find_sum(values: List[int], target: int):
    s = [entry(item, index) for index, item in enumerate(values)]
    heapq.heapify(s)
    t = [entry(item, index, True) for index, item in enumerate(values)]
    heapq.heapify(t)
    a = heapq.heappop(s)
    b = heapq.heappop(t)

    while a != b:
        if a.value + b.value == target:
            i, j = a.idx, b.idx
            if i > j:
                i, j = j, i
            return i, j

        if a.value + b.value < target:
            a = heapq.heappop(s)
        else:
            b = heapq.heappop(t)

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
        print(f'test failed: Got {result}:  {t}', file=sys.stderr)

print(f'{count} tests passed')
