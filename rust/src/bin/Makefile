# Hacky makefile to demonstrate building the crates by hand.
# These should be built via 'cargo build' in the top level.


SOURCES := $(filter-out misc.rs, $(wildcard *.rs))
EXE := $(patsubst %.rs,%,$(SOURCES))

all: $(EXE) libwrp_examples.rlib

%: %.rs libwrp_examples.rlib
	rustc -C debug-assertions=no -C opt-level=3 --extern wrp_examples=libwrp_examples.rlib $<

libwrp_examples.rlib: ../lib.rs
	rustc --crate-type=lib $< -o $@

clean:
	rm -f $(EXE) *.rlib
