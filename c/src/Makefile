
CPPFLAGS = -I ../include
# CFLAGS=--coverage -g -O0
LDFLAGS = -L../lib
LDLIBS = -lxutil -lring-buffer -lstack -lsqlite3 -lm

SOURCES := $(wildcard *.c)
EXE := $(subst .c,,$(SOURCES))

.PHONY: libs clean all

all: libs $(EXE)

libs:
	cd ../lib; make

check: udp-client udp-server
	@for path in ../test/*-test; do \
		file=$$(basename $$path); \
		test "$$file" = coverage-test && continue; \
		make $${file%-test} > /dev/null; \
		$$path || exit 1; \
	done

clean:
	rm -f $(EXE) *.gcno *.gcda *.o
	cd ../lib; make clean
