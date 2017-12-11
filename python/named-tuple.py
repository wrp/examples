#!/usr/bin/env python2.7

from collections import namedtuple

Thing = namedtuple('Thingy', ['one', 'two'])

a = Thing('foo','bar')

c, b = a
print "a=", a  # A is a Thingy
print "b=", b  # b is 'bar'
print "c=", c  # c is 'foo'

