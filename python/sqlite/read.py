#!/usr/bin/env python

import sqlite3
import datetime

path = 'sqlite.example'

db = sqlite3.connect(path)
c = db.cursor()

c.execute('SELECT name from sqlite_master WHERE type="table";')
print c.fetchall()

c.execute('SELECT * from ops')
print c.fetchall()

db.commit()
db.close()


