#!/usr/bin/python

# Validate json
import sys
import json
d = json.load(sys.stdin)

print d
print d.get('foo','default')

#k = {'foo':'bar'}

#print json.dumps(k)

#data = sys.stdin.read()
#d = json.loads(data)

