#!/usr/bin/env python

# Demonstrat that it is 'elif', not 'else', or 'else if'
import sys
if len(sys.argv) == 1:
    print 1
elif len(sys.argv) == 2:
    print 2
else:
    print 'big'
