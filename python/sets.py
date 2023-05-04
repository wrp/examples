#!/usr/bin/env python
# various set operations (2.7)

A = {0, 2, 4, 6, 8, 2};
B = {1, 2, 3, 4, 5};

print("A:                         A = ", A)
print("B:                         B = ", B)
print("type:                type(A) = ", type(A))
print("Union:                 A | B = ", A | B)
print("Intersection:          A & B = ", A & B)
print("Difference :           A - B = ", A - B)
print("Symmetric difference : A ^ B = ", A ^ B)
