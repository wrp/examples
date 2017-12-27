#!/usr/bin/python

from qux import f

try:
    print 'importing module {} from file {}'.format( __name__, __file__)
    f()
except Exception as e:
    print "raised ", e

True
