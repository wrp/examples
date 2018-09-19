#!/usr/bin/env python

# Demonstrate basic click usage.
# Sample calls:
# $ ./hello.py greet you
# Hello, you!
# $ ./hello.py greet -g 'bonjour' you
# bonjour, you!

import click

@click.group()
def cli():
    pass

@cli.command(short_help='short help')
@click.argument('name')
@click.option('-g', '--greeting')
@click.option('-c', is_flag=True, help='use all caps')
def greet(name, greeting, c):
    """Hello world"""

    if c:
        name = name.upper()

    print "%s, %s!"%( greeting, name )

if __name__ == '__main__':
    cli()
