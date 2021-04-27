EXE = $(patsubst %.rs,%,$(wildcard *.rs))

.PHONY: last
last:
	@f=$$(ls -t *.rs | sed 1q); \
	rustc -o a.out "$$f" && \
	./a.out


all: $(EXE)

%: %.rs
	rustc $<

clean:
	rm -f $(EXE) a.out
