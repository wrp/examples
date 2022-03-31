#!/usr/bin/env python

from dis import dis


def my_list():
    x = [10, 20, 30, "abc"]
    y = x[0]


def my_tuple():
    x = (10, 20, 30, "abc")
    y = x[0]


dis(my_list)
dis(my_tuple)
