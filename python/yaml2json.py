#!/usr/bin/env python

import sys
import yaml
import json

def main(args):

    for x in args:
        try:
            json.dump(yaml.load(open(x,'r').read()), sys.stdout, indent = 8)
        except IOError as e:
            if e.errno in [2,63]:
                json.dump(yaml.load(x), sys.stdout, indent = 8)
            else:
                raise e
                


data = r'''
foo:
    bar: value
    scalar: >
        Scalar content can be written in block notation, using a '>'
    nospace: "this string has\
        no space between 'has' and 'no'"
view: 
    query: >
        SELECT  * EXCEPT (etl_row_num)
            FROM (
                SELECT *, ROW_NUMBER() OVER (PARTITION BY producerHeader.uuid ORDER BY producerHeader.timestamp DESC) etl_row_num\n  FROM\n    `micro-bus-121218.queuing_kafka_stingray.billcom_money_movement_report`\n  WHERE\n
    "query2": >
        "SELECT  * EXCEPT (etl_row_num) \n FROM ( \n SELECT "
    "query3":
        "SELECT  * EXCEPT (etl_row_num) \n FROM ( \n SELECT "
    useLegacySql: false 
'''

if __name__ == '__main__':
    if len(sys.argv) > 1:
        main(sys.argv[1:])
    else:
        main([data])
