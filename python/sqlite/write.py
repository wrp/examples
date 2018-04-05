#!/usr/bin/env python


import sqlite3
import datetime

path = 'sqlite.example'

today = datetime.date.today()

db = sqlite3.connect(path)
c = db.cursor()

c.execute('CREATE TABLE ops (opdate date, project varchar(256))')
c.execute('CREATE INDEX opdate_index ON ops (opdate);')
c.execute("INSERT INTO ops VALUES ('%s', '%s');" % (str(today), 'test'))
c.execute("INSERT INTO ops VALUES ('%s', '%s');" % (0, 'null'))

db.commit()
db.close()


