#!/usr/bin/env python

from ConfigParser import ConfigParser
from StringIO import StringIO

C = ConfigParser()
C.read('example.ini')

print C.sections()
print C.get('SectionOne','status')  # Case insenstive
print C.get('SectionTwo','name')  # Case insenstive


a = StringIO('''
[foo]
status: yes
Name = bob

[bar]
status: no
Name = doug
''')

C.readfp(a)
print C.get('bar','status')  # Case insenstive
print C.get('foo','name')  # Case insenstive
