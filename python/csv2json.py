#!/usr/bin/env python

import csv
import json
import sys

reader = csv.DictReader(sys.stdin)
title = reader.fieldnames

print title

rows = []
for row in reader:
    rows.extend([{title[i]:row[title[i]] for i in range(len(title))}])

sys.stdout.write(json.dumps(rows))
    
sys.stdout.write('\n')

