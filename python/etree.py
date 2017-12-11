#!/usr/bin/python2.7
import xml.etree.ElementTree as ET
a = '''
<!doctype html>

<head>
</head>
<body>
</body>
'''
root = ET.fromstring(a)
print dir(root)
