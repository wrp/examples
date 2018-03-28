#!/usr/bin/env python

import os
import sys

try:
    start = sys.argv[1]
except IndexError:
    start = '.'

for root, dirs, files in os.walk(start):
#    print 'root:', root
#    print 'dirs:', dirs
#    print 'files:', files
    for f in map(lambda f: os.path.join(root, f), filter(lambda x: x.endswith('.pyc'), files)):
        print f
