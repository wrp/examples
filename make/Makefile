
foo = recursively expanded variable
bar := simply expanded variable
# baz ::= posix mandated simply expanded variable
qux ?= conditional assignment
# qan != echo shell expanded var

all:
	echo foo
	echo test :+ $${TMP:+TMP is $(TMP)}


ifeq '$(qux)' 'foo'

else ifeq '$(qux)' 'qux'
	$(error qux "$(env)")
else
	$(warn qux not set)
endif
 

other:
	echo foo = $(foo)
	echo bar = $(bar)
	echo baz = $(baz)
	echo qux = $(qux)
	echo qan = $(qan)

