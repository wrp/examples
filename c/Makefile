
.PHONY: all check

targets = \
	main/hello \
	main/calc \
	main/cat \

all: $(targets)

check:
	bazel test test/calc_test

%:
	bazel build $@
