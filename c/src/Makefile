
CPPFLAGS = -I ../include
LDFLAGS = -L../lib
LDLIBS = -lxutil -lring-buffer -lstack -lsqlite3

check: udp-client udp-server
	@for path in ../test/*-test; do \
		file=$$(basename $$path); \
		test "$$file" = coverage-test && continue; \
		make $${file%-test} > /dev/null; \
		$$path || exit 1; \
	done
