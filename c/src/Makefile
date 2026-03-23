


.PHONY: clean all check install
all: builddir/build.ninja
	meson compile -C builddir $${V+--verbose}

builddir/build.ninja:
	meson setup builddir

install: builddir/build.ninja
	meson install -C builddir

check: builddir/build.ninja
	meson test -C builddir

clean:
	rm -rf builddir
