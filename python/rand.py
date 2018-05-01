#!/usr/bin/env python
import string
import random
print ''.join(random.choice(string.printable) for _ in range(16))
