
.PHONY: all

targets = \
	main/hello \
	main/calc \
	main/cat \

all: $(targets)

%:
	bazel build $@
