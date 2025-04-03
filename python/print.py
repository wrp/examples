#!/usr/bin/env python3

import sys
import time

print("stderr", file=sys.stderr)
print("noeol", end='')  # keyword newline endl eol
print("stdout")

h = "hello"
w = "world"
p = 3.14159265
print(f"{h}, {w}!")
print(f"{h}, {w:20}!")  # 20 wide
print(f"{h!a}, {w!r}!")  # Apply ascii() and repr()
print(f"pi =~ {p:6.2f}!")
print("numbered: {1}, {0}!".format(h, w))  # number the args
print("named: {foo}, {bar}!".format(foo=h, bar=w))  # named args

a = {"foo": 45.12345, "bar": 12.34567}
b = {"baz": 45.12345, "qux": 12.34567}
print("{0[foo]:.2f} (45.12) {1[qux]:.2f} (12.35)".format(a, b))

print('This line will be cleared an over-written', end='\r')
time.sleep(1)
print('Hello, world!', end='\033[K\n')
