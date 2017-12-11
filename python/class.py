#!/usr/bin/python2.7

class Foo(object):
    bar = 5
    def __init__(self, x):
        self.x = x
        self.bar += x

    def name(self):
      return "%d,%d" % (self.x, self.bar)


a = Foo(7)
print a.name()

b = Foo(9)
print b.name()
print a.name()
