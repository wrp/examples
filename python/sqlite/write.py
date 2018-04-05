#!/usr/bin/env python


import sqlite3
import datetime

path = 'sqlite.example'

today = datetime.date.today()

with sqlite3.connect(path) as db:
    c = db.cursor()
    c.execute('CREATE TABLE ops (id integer NOT NULL PRIMARY KEY, name varchar(256))')
    c.execute("INSERT INTO ops VALUES ('%s', '%s');" % (1, 'test'))
    c.execute("INSERT INTO ops VALUES ('%s', '%s');" % (2, 'two'))
    c.execute("INSERT INTO ops VALUES ('%s', '%s');" % (3, 'three'))

# db not closed by with clause: counterintuitive

# execute with a failure to demonstrate rollback
try:
    with db:
        c.execute("INSERT INTO ops VALUES ('%s', '%s');" % (4, 'four'))
        c.execute("INSERT INTO ops VALUES ('%s', '%s');" % (2, 'null'))  # Throws

except sqlite3.IntegrityError as e:
    print 'transaction rolled back', e



