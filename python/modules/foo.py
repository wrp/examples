#!/usr/bin/python

try:
    print 'importing foo from %s' % __file__
    print 'name (in foo) is {}'.format(__name__)
except Exception as e:
    print "raised ", e

True
