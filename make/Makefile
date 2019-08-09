
foo = recursively expanded variable
bar := simply expanded variable
# baz ::= posix mandated simply expanded variable
qux ?= conditional assignment
# qan != echo shell expanded var

all:
	@echo Running rules in Makefile
	#
	# Whitespace is preserved in both expansions here:
	echo test :+ $${TMP:+TMP is $(TMP) and ${TMP}}
	@echo foo = $(foo)
	@echo bar = $(bar)
	@echo baz = $(baz)
	@echo qux = $(qux)
	@echo qan = $(qan)
