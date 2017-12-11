#!/usr/bin/env python

from ConfigParser import ConfigParser

C = ConfigParser()
C.read('example.ini')

print C.sections()
print C.get('SectionOne','status')  # Case insenstive
print C.get('SectionTwo','name')  # Case insenstive
