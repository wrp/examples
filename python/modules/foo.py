#!/usr/bin/python

try:
    print 'importing foo from %s' % __file__
except Exception as e:
    print "raised ", e

True
