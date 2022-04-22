#!/usr/bin/env python2.7

from collections import namedtuple

Thing = namedtuple('Thingy', ['one', 'two'])
a = Thing('foo','bar')
c, b = a

print "a=", a  # a is a Thingy
print "b=", b  # b is 'bar'
print "c=", c  # c is 'foo'
print "a.one = ", a.one
print "a.two = ", a.two



# Note that the naming of the fields can also done with a
# string containing a space or comma separated list
SThing = namedtuple('Thingy', 'one, two')
a = SThing(5, 6)
print('a.one = {}'.format(a.one))
