#!/usr/bin/env python

import sys
import yaml
import json

# Simple version:
# json.dump(yaml.load(sys.stdin.read()), sys.stdout, indent = 8)


def main(fh):
    try:
        json.dump(
                yaml.load(fh.read(), Loader=yaml.SafeLoader),
                sys.stdout,
                indent=8,
        )
    except:
        json.dump(yaml.load(x), sys.stdout, indent=8)
    print()


if __name__ == "__main__":
    if len(sys.argv) > 1:
        for x in sys.argv[1:]:
            with open(x, "r") as f:
                main(f)
    else:
        main(sys.stdin)
