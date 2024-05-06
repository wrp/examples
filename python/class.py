#!/usr/bin/env python3

import types

class Foo: pass
Foo.bar = 5

def n(self):
        rv = f"x = {self.x}, bar = {self.bar}"
        if hasattr(self, "baz"):
            rv += f", baz = {self.baz}"
        return rv
Foo.__str__ = n

def n(self, x):
        self.x = x
        self.bar += x
        self.map = {}
Foo.__init__ = n



a = Foo(7)
print(a)

b = Foo(9)
print(b)
a.bar = 9
a.baz = 11
print(a)

