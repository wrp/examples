
CPPFLAGS = -I ../include
CFLAGS=--coverage -g -O0
LDFLAGS = -L../lib
LDLIBS = -lxutil -lring-buffer -lstack -lsqlite3

SOURCES := $(wildcard *.c)
EXE := $(subst .c,,$(SOURCES))

all: $(EXE)

check: udp-client udp-server
	@for path in ../test/*-test; do \
		file=$$(basename $$path); \
		test "$$file" = coverage-test && continue; \
		make $${file%-test} > /dev/null; \
		$$path || exit 1; \
	done

clean:
	rm -f $(EXE)
