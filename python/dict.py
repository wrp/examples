#!/usr/bin/env python

import json

class dqdict(dict):
    def __str__(self):
        return json.dumps(self)

a = dqdict()
a['foo'] = 'bar'
a[3] = 'aux'

print a
