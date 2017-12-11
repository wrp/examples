#!/usr/bin/python

import foo
import baz
import sys

print 'sys.modules:', sys.modules['foo']
print 'file is ', foo.__file__  # Full path to either foo.py or foo.pyc
print 'name from bar is', foo.__name__
