#!/usr/bin/env python

# Demonstrate 'elif', not 'elsif', or 'else if'
import sys
if len(sys.argv) == 1:
    print 1
elif len(sys.argv) == 2:
    print 2
else:
    print 'big'
