
.PHONY: all check

cache_dir = /tmp/build_output
bazel = docker run \
	-e USER=$$(id -u) \
	-u $$(id -u) \
	-v $(PWD):/src/workspace \
	-v $(cache_dir):$(cache_dir) \
	-w /src/workspace \
	gcr.io/bazel-public/bazel:latest \
	--output_user_root=$(cache_dir)

targets = \
	main/hello \
	main/calc \
	main/cat \

all: $(targets)

$(cache_dir):
	mkdir -p $(cache_dir)

main/%:
	$(bazel) build $@

test/%:
	$(bazel) test $@
