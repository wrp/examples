#!/usr/bin/python2
from lxml import etree

a='''
<!doctype html>

<html lang="en">
<head>
    <meta charset="utf-8">
</head>
'''
root = etree.fromstring(a)
print root
