# Demonstrate various GNU make functionality.
# TODO: at some point, only put things here that
# are non-standard, and put the standard things
# in Makefile.

foo = recursively expanded variable
bar := simply expanded variable
# baz ::= posix mandated simply expanded variable
qux ?= conditional assignment
qan != echo shell expanded var

all:
	@echo 'Running rules in GNUmakefile (try bsdmake, or use -f to avoid)'
	@echo foo = $(foo)
	@echo bar = $(bar)
	@echo baz = $(baz)
	@echo qux = $(qux)
	@echo qan = $(qan)
