#!/usr/bin/env python

import sys
from itertools import zip_longest
from collections import namedtuple

class test_case:
    def __init__(self, record: namedtuple):
        # input1, input2, output = line.strip().split('\t')
        self.input1 = list(map(int, record.input1.strip('[]').split(',')))
        self.input2 = list(map(int, record.input2.strip('[]').split(',')))
        self.output = list(map(int, record.output.strip('[]').split(',')))
        if any( 0 > x or x > 9 for x in self.input1 + self.input2 + self.output):
            raise ValueError

class Solution:
    def addTwoNumbers(self, l1, l2):
        output = []
        carry = 0
        for a, b in zip_longest(l1, l2, fillvalue='0'):
            v = int(a) + int(b) + carry
            if v > 9:
                v -= 10
                carry = 1
            else:
                carry = 0
            output.append(v)
        if carry:
            output.append(1)
        return output

def main():
    headers = sys.stdin.readline().strip().split('\t')
    record = namedtuple('record', headers)
    count = 0
    for line in sys.stdin:
        t = test_case(record(*line.strip().split('\t')))
        s = Solution().addTwoNumbers(t.input1, t.input2)
        if s != t.output:
            print(f'FAILED: {t.output}.  Got {s}', file=sys.stderr)
        else:
            count += 1

    print(f'{count} tests passed')

if __name__ == '__main__':
    main()
