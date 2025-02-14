# Class Foo used in script 'class'

def initialize(x):
    print(f'initializing{x}')
    return x



class Foo:
    bar = initialize(5)  # Initialization of class attribute happens only once

def __meth(self: Foo):
    print(f'A do nothing method on {self}')

def __new(cls: Foo, *args, **kwargs):
    print(f'Foo.__new__ called with {args} {kwargs}')
    return super(Foo, cls).__new__(cls)

def __str(self: Foo) -> str:
    rv = f"x = {self.x}, bar = {self.bar}"
    if hasattr(self, "baz"):
        rv += f", baz = {self.baz}"
    return rv

Foo.__new__ = __new
Foo.__str__ = __str
Foo.meth = __meth
