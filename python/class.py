#!/usr/bin/env python3

import types


def initialize(x):
    print(f'initializing{x}')
    return x

class Foo:
    def __new__(cls, *args, **kwargs):
        print(f'Foo.__new__ called with {args} {kwargs}')
        return super().__new__(cls)

    bar = initialize(5)  # This is only called once

def n(self):
        rv = f"x = {self.x}, bar = {self.bar}"
        if hasattr(self, "baz"):
            rv += f", baz = {self.baz}"
        return rv
Foo.__str__ = n

def n(self, x, **kwargs):
        self.x = x
        print(f'In init befor assign: {Foo.bar}  {self.bar}')
        self.bar += x
        print(f'In init after assign: {Foo.bar}  {self.bar}')
        self.map = {}
Foo.__init__ = n



print(f'Before creating A, Foo.bar = {Foo.bar}')
a = Foo(7)  # __init__  incrmements the instance variable bar

# After creating a, Foo.bar is NOT changed
print(f'After creating A, Foo.bar = {Foo.bar}')


print(a)  # Calling print invokes __str__ which  does not change anything

b = Foo(9, name='b')
print(b)
a.bar = 9
a.baz = 11
print(a)
