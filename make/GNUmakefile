# Demonstrate various GNU make functionality.
# See Makefile for things which work in both gnu
# and bsd make

# Add prereqs to each target (allegedly valid in gnumake 4.3, but haven't
# had a chance to test
.EXTRA_PREREQS+=$(foreach mk, ${MAKEFILE_LIST}, $(abspath ${mk}), bar)

.PHONY: all

all:
	@echo arguments to make: $(MAKECMDGOALS)
	@echo 'Running rules in GNUmakefile (try bsdmake, or use -f to avoid)'


foo:
	echo foo newer prerequisites: $?
	echo foo all prerequisites: $^
