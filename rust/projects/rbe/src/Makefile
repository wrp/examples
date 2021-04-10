EXE = $(patsubst %.rs,%,$(wildcard *.rs))

all: $(EXE)

%: %.rs
	rustc $<

clean:
	rm -f $(EXE)
