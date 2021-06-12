
SOURCES := $(wildcard *.rs)
EXE := $(patsubst %.rs,%,$(SOURCES))

all: $(EXE)

%: %.rs
	rustc $<

clean:
	rm -f $(EXE)
