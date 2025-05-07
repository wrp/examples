#!/usr/bin/env python
# various set operations (2.7)

A = {0, 2, 4, 6, 8, 2};
B = {1, 2, 3, 4, 5};

assert(isinstance(A, set))
assert( A | B == {0, 1, 2, 3, 4, 5, 6, 8})
assert( A & B == {2, 4})
assert( A - B == {0, 6, 8})
assert( A ^ B == {0, 1, 3, 5, 6, 8})


'''

Help on class frozenset in module builtins:

class frozenset(object)
 |  frozenset() -> empty frozenset object
 |  frozenset(iterable) -> frozenset object
 |
 |  Build an immutable unordered collection of unique elements.
 |
 |  Methods defined:
 |  __and__(self, value, /)          Return self&value.
 |  __contains__(...)                x.__contains__(y) <==> y in x.
 |  __eq__(self, value, /)           Return self==value.
 |  __ge__(self, value, /)           Return self>=value.
 |  __getattribute__(self, name, /)  Return getattr(self, name).
 |  __gt__(self, value, /)           Return self>value.
 |  __hash__(self, /)                Return hash(self).
 |  __iter__(self, /)                Implement iter(self).
 |  __le__(self, value, /)           Return self<=value.
 |  __len__(self, /)                 Return len(self).
 |  __lt__(self, value, /)           Return self<value.
 |  __ne__(self, value, /)           Return self!=value.
 |  __or__(self, value, /)           Return self|value.
 |  __rand__(self, value, /)         Return value&self.
 |  __reduce__(...)                  Return state information for pickling.
 |  __repr__(self, /)                Return repr(self).
 |  __ror__(self, value, /)          Return value|self.
 |  __rsub__(self, value, /)         Return value-self.
 |  __rxor__(self, value, /)         Return value^self.
 |  __sizeof__(...)                  Return size memory, in bytes
 |  __sub__(self, value, /)          Return self-value.
 |  __xor__(self, value, /)          Return self^value.
 |  copy(...)                        Return a shallow copy of a set.
 |  difference(...)     Return the difference of two or more sets as a new set.
 |      (i.e. all elements that are in this set but not the others.)
 |  intersection(...)   Return the intersection of two sets as a new set.
 |  isdisjoint(...)     Return True if two sets have a null intersection.
 |  issubset(...)       Report whether another set contains this set.
 |  issuperset(...)     Report whether this set contains another set.
 |  symmetric_difference(...) Return the symmetric difference of two sets as a new set.
 |      (i.e. all elements that are in exactly one of the sets.)
 |  union(...)        Return the union of sets as a new set.
 |      (i.e. all elements that are in either set.)
 |  ----------------------------------------------------------------------
 |  Class methods defined here:
 |
 |  __class_getitem__(...) from builtins.type
 |      See PEP 585
 |
 |  ----------------------------------------------------------------------
 |  Static methods defined here:
 |
 |  __new__(*args, **kwargs) from builtins.type
 |      Create and return a new object.  See help(type) for accurate signature.


Methods that are only defined for sets (not frozensets, as above)
diff help(sets) help(frozensets)

<  |  Build an unordered collection of unique elements.

<  |  __iand__(self, value, /) Return self&=value.
<  |  __init__(self, /, *args, **kwargs) Initialize self.  See help(type(self)) for accurate signature.
<  |  __ior__(self, value, /) Return self|=value.
<  |  __isub__(self, value, /) Return self-=value.
<  |  __ixor__(self, value, /) Return self^=value.
      __hash__ is NOT defined.  Sets are not hashable
<  |  add(...) Add an element to a set.
<  |      This has no effect if the element is already present.
<  |  clear(...) Remove all elements from this set.
<  |  difference_update(...)      Remove all elements of another set from this set.
<  |  discard(...)       Remove an element from a set if it is a member.
<  |      Unlike set.remove(), the discard() method does not raise
<  |      an exception when an element is missing from the set.
<  |
<  |  intersection_update(...)
<  |      Update a set with the intersection of itself and another.
<  |
<  |  pop(...)
<  |      Remove and return an arbitrary set element.
<  |      Raises KeyError if the set is empty.
<  |
<  |  remove(...)
<  |      Remove an element from a set; it must be a member.
<  |      If the element is not a member, raise a KeyError.
<  |
<  |  symmetric_difference_update(...)
<  |      Update a set with the symmetric difference of itself and another.

<  |  update(...)
<  |      Update a set with the union of itself and others.
<  |
<  |  ----------------------------------------------------------------------
<  |  Data and other attributes defined here:
<  |
<  |  __hash__ = None
'''
