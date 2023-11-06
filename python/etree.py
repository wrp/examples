#!/usr/bin/env python
import xml.etree.ElementTree as ET
a = '''<?xml version="1.0"?>
<data>
<head>
</head>
<body>
</body>
</data>
'''
root = ET.fromstring(a)
print(dir(root))
