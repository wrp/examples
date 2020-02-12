#!/usr/bin/env python
# various set operations (2.7)

A = {0, 2, 4, 6, 8};
B = {1, 2, 3, 4, 5};

print "type: ",  type(A)
print "Union :", A | B
print "Intersection :", A & B
print "Difference :", A - B
print "Symmetric difference :", A ^ B
