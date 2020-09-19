CC=gcc
CFLAGS = -Wall -Wextra
LDLIBS = -lncursesw -lutil
CPPFLAGS=-I/usr/local/opt/ncurses/include
LDFLAGS=-L/usr/local/opt/ncurses/lib

one-byte: wrappty show-key
	@trap exit SIGINT; \
	for i in $$(seq 0 255); do \
		printf "%8d:" $$i; perl -e "print pack('c', $$i)" | ./wrappty ./show-key& wait; \
	done | tee one-byte
