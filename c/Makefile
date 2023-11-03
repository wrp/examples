
.PHONY: all check

targets = \
	main/hello \
	main/calc \
	main/cat \

all: $(targets)

main/%:
	bazel build $@

test/%:
	bazel test $@
