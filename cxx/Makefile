CXXFLAGS ?= -fno-elide-constructors -std=c++23

.PHONY: all

all: meson.build builddir/build.ninja
	meson compile -C builddir $${V+--verbose}

meson.build: meson.build.in
	head=$$(git describe --always); \
	sed -e 's/@version@/'"$${head}/" meson.build.in > meson.build
	for suffix in cpp cc; do \
		for file in *.$$suffix; do \
			base=$${file%.$$suffix}; \
			printf '%s\n' "executable('$${base}', '$$file')"; \
		done; \
	done >> meson.build


builddir/build.ninja:
	meson setup builddir

install: builddir/build.ninja
	meson install -C builddir

check: all builddir/build.ninja
	meson test -C builddir

clean:
	git clean -xdff
