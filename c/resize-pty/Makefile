CC=gcc
CFLAGS = -Wall -Wextra
LDLIBS = -lncursesw -lutil
CPPFLAGS=-I/usr/local/opt/ncurses/include
LDFLAGS=-L/usr/local/opt/ncurses/lib

list = 15 16 17 18 19 20 21 22 23 24
one-byte: wrappty show-key
	@trap exit SIGINT; \
	for i in $$(seq 0 255); do \
		printf "%8d: " $$i; perl -e "print pack('c', $$i)" | ./wrappty ./show-key& wait; \
	done 2>&1 | tee one-byte

two-byte: wrappty show-key
	@trap exit SIGINT; \
	for j in $(list); do \
	for i in $$(seq 0 255); do \
		printf "%02x%02x: " $$j $$i; perl -e "print pack('cc', $$j, $$i)" | ./wrappty ./show-key& wait; \
	done; done 2>&1 | tee two-byte

three-byte-new: wrappty show-key
	@trap exit SIGINT; \
	for k in $(list); do \
	for j in $(list); do \
	for i in $(list); do \
		printf "%02x%02x%02x: " $$k $$j $$i; perl -e "print pack('ccc', $$k, $$j, $$i)" | ./wrappty ./show-key& wait; \
	done; done; done 2>&1 | tee three-byte-new
