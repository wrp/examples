
foo = recursively expanded variable
bar := simply expanded variable
# baz ::= posix mandated simply expanded variable
qux ?= conditional assignment
# qan != echo shell expanded var

ifeq ($(foo), foo)
qaz := foo is foo
else
qaz := foo is not foo
endif



ND ?= $(notdir /this/is/a/path)

all:
	echo not dir =  $(ND)
	echo Running rules in Makefile
	#
	# Whitespace is preserved in both expansions here:
	echo test :+ $${TMP:+TMP is $(TMP) and ${TMP}}
	echo foo = $(foo)
	echo bar = $(bar)
	echo baz = $(baz)
	echo qux = $(qux)
	echo qan = $(qan)
	echo qaz = $(qaz)
	echo  target name: $@

quotes:
	# Demonstrate how to embed newlines into an argument
	echo foo "$$( printf '%s\n%s\n%s\n' \
		'#!/bin/bash' \
		'cd /etc/vm/startup' \
		'exec ./start.sh "$@"' \
	)"
