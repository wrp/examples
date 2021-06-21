
SOURCES := $(wildcard *.rs)
EXE := $(patsubst %.rs,%,$(SOURCES))

all: $(EXE) libmisc.rlib

%: %.rs libmisc.rlib
	rustc --extern misc=libmisc.rlib $<

lib%.rlib: %.rs
	rustc --crate-type=lib  $<

clean:
	rm -f $(EXE)
