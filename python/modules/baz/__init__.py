#!/usr/bin/python

try:
    print 'importing module {} from file {}'.format( __name__, __file__)
except Exception as e:
    print "raised ", e

True
