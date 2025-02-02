

SOURCES := $(wildcard *.rs)
EXE := $(patsubst %.rs,%,$(SOURCES))

all: $(EXE)

%: %.rs libtext_io.rlib
	rustc --extern text_io=libtext_io.rlib $<


libtext_io.rlib:
	cd ../../rust && make
	ln -s $$(ls -t ../../rust/target/debug/deps/libtext_io-*.rlib | sed 1q) $@
