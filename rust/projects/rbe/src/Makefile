EXE = $(patsubst %.rs,%,$(wildcard *.rs))

all: $(EXE)

%: %.rs
	rustc -o a.out $<

clean:
	rm -f $(EXE)
