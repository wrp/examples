
SOURCES := $(filter-out misc.rs, $(wildcard *.rs))
EXE := $(patsubst %.rs,%,$(SOURCES))

all: $(EXE) libmisc.rlib

%: %.rs libmisc.rlib
	rustc -C debug-assertions=no -C opt-level=3 --extern misc=libmisc.rlib $<

lib%.rlib: %.rs
	rustc --crate-type=lib  $<

clean:
	rm -f $(EXE) *.rlib
