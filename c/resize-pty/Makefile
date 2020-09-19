CC=gcc
CFLAGS = -Wall -Wextra
LDLIBS = -lncursesw -lutil
CPPFLAGS=-I/usr/local/opt/ncurses/include
LDFLAGS=-L/usr/local/opt/ncurses/lib

one-byte: wrappty show-key
	@trap exit SIGINT; \
	for i in $$(seq 0 255); do \
		printf "%8d: " $$i; perl -e "print pack('c', $$i)" | ./wrappty ./show-key& wait; \
	done 2>&1 | tee one-byte

two-byte: wrappty show-key
	@trap exit SIGINT; \
	for j in 15 17 19 22 23 24 25 26 27 28 29 30 31 32; do \
	for i in $$(seq 0 255); do \
		printf "%02x%02x: " $$j $$i; perl -e "print pack('cc', $$j, $$i)" | ./wrappty ./show-key& wait; \
	done; done 2>&1 | tee two-byte

three-byte: wrappty show-key
	@trap exit SIGINT; \
	for k in 15 17 19 22 23 24 25 26 27 28 29 30 31 32; do \
	for j in 15 17 19 22 23 24 25 26 27 28 29 30 31 32; do \
	for i in $$(seq 0 255); do \
		printf "%02x%02x%02x: " $$k $$j $$i; perl -e "print pack('ccc', $$k, $$j, $$i)" | ./wrappty ./show-key& wait; \
	done; done; done 2>&1 | tee three-byte
