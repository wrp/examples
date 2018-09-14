#!/usr/bin/env python

import click

@click.group()
def cli():
	pass

@cli.command(short_help='short help')
@click.argument('name')
@click.option('-c', is_flag=True, help='use all caps')
def greet(name, c):
	"""Hello world"""
	if c:
		name = name.upper()
	print "Hello, %s!"%(name)

if __name__ == '__main__':
	cli()
