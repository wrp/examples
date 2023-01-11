#!/usr/bin/env python

import json

class dqdict(dict):
    def __str__(self):
        return json.dumps(self)

a = dqdict()
a['foo'] = 'bar'
a[3] = 'aux'
print(a)

def mutate(a):
    '''Dictionaries are passed by reference.  This mutates in the caller.'''
    a['foo'] += 5

b = {'foo':1}
mutate( b )       # value increments
mutate( dict(b) ) # To avoid mutation, pass a copy

# iterate over a dictionary
for k, v in b.items():
    print(f'b[{k}] = {v}')
