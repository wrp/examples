

SRCS = $(wildcard *.j2)
OBJ = $(patsubst %.j2, %-output, $(SRCS))

.PHONY: all
all: $(OBJ)

%-output: %.j2
	./driver $< > $@

clean:
	rm -f $(OBJ)
