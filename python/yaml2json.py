#!/usr/bin/env python

import sys
import yaml
import json

# Simple version:
# json.dump(yaml.load(sys.stdin.read()), sys.stdout, indent = 8)


def main(args):

    for x in args:
        try:
            json.dump(yaml.load(open(x,'r').read(), Loader=yaml.SafeLoader), sys.stdout, indent = 8)
        except IOError as e:
            if e.errno in [2,63]:
                json.dump(yaml.load(x), sys.stdout, indent = 8)
            else:
                raise e


if __name__ == '__main__':
    if len(sys.argv) > 1:
        main(sys.argv[1:])
    else:
        main(['/dev/stdin/'])
