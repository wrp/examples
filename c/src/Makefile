

# For non-meson builds, allowing the user to
# do 'make foo' in the top-level dir
CPPFLAGS=-Ibuilddir
LDFLAGS=-Lbuilddir
LDLIBS=-lring-buffer -lstack


.PHONY: clean all check install
all: builddir/build.ninja
	meson compile -C builddir $${V+--verbose}

builddir/build.ninja:
	meson setup builddir

install: builddir/build.ninja
	meson install -C builddir

check: all builddir/build.ninja
	meson test -C builddir

clean:
	rm -rf builddir
