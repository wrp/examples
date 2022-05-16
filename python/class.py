#!/usr/bin/env python3


class Foo:
    bar = 5

    def __init__(self, x):
        self.x = x
        self.bar += x

    def __str__(self):
        rv = f"x = {self.x}, bar = {self.bar}"
        if hasattr(self, "baz"):
            rv += f", baz = {self.baz}"

        return rv


a = Foo(7)
print(a)

b = Foo(9)
print(b)
a.bar = 9
a.baz = 11
print(a)

print(a.bar)
print([x for x in a.get('kk', None)])
