
CPPFLAGS = -I ../include
LDFLAGS = -L../lib
LDLIBS = -lxutil -lring-buffer -lstack -lsqlite3

check: udp-client udp-server
	for path in ../test/*-test; do \
		file=$$(basename $$path); \
		make $${file%-test}; \
		echo "executing $$path"; \
		$$path; \
	done
