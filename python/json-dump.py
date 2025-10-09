#!/usr/bin/env python

# Trivial example demonstrating json.dump
import sys
import json

def main(a):
        json.dump(
                a,
                sys.stdout,
                indent=8,
        )

a = [ ('foo', 'bar'), { 'baz': 'qux'} ]

main(a)
print()
