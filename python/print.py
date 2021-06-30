#!/usr/bin/env python3

import sys
print('stderr', file=sys.stderr)
print('stdout')

h = 'hello'
w = 'world'
print(f'{h}, {w}!')
print('{}, {}!'.format(h, w))
