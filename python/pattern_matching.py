#!/usr/bin/env python

# New in 3.10

# See:
#    PEP 634: https://peps.python.org/pep-0634/
#    https://docs.python.org/3/reference/compound_stmts.html#match

import sys
from enum import Enum

def main():
    Color = Enum('Color', ['RED', 'GREEN', 'BLUE'])

    color = Color.GREEN

    match color:
        case Color.RED:
            print("I see red!")
        case Color.GREEN:
            print("Grass is green")
        case Color.BLUE:
            print("I'm feeling the blues :(")
        case _:
            print("DEFAULT")
