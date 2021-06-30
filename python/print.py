#!/usr/bin/env python3

import sys
print('stderr', file=sys.stderr)
print('stdout')

h = 'hello'
w = 'world'
p=3.14159265
print(f'{h}, {w}!')
print(f'{h}, {w:20}!')
print(f'pi =~ {p:6.2f}!')
print('{}, {}!'.format(h, w))
