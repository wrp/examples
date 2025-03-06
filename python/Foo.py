# Class Foo used in script 'class'

from __future__ import annotations


class Foo:
    bar = 0  # Initialization of class attribute happens only once


    def __init__(self: Foo, x: int, **kwargs: dict):
            self.x = x
            self.bar += x  # Does not modify class instance
            self.map = {}
            assert Foo.bar == 0

def __meth(self: Foo):
    print(f'A do nothing method on {self}')

def __new(cls: Foo, *args, **kwargs):
    print(f'Foo.__new__ called with {args} {kwargs}')
    s = super(Foo, cls)
    assert s.__self_class__.bar == 0
    return s.__new__(cls)

def __str(self: Foo) -> str:
    rv = f"x = {self.x}, bar = {self.bar}"
    if hasattr(self, "baz"):
        rv += f", baz = {self.baz}"
    return rv

Foo.__new__ = __new
Foo.__str__ = __str
Foo.meth = __meth
