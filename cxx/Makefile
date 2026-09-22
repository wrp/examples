CXXFLAGS ?= -fno-elide-constructors -std=c++23

.PHONY: all
BUILDDIR := builddir

all: meson.build $(BUILDDIR)/build.ninja
	meson compile -C $(BUILDDIR) $${V+--verbose}

meson.build: meson.build.in
	head=$$(git describe --always); \
	sed -e 's/@version@/'"$${head}/" meson.build.in > meson.build
	for suffix in cpp cc; do \
		for file in *.$$suffix; do \
			base=$${file%.$$suffix}; \
			printf '%s\n' "executable('$${base}', '$$file')"; \
		done; \
	done >> meson.build


$(BUILDDIR)/build.ninja:
	meson setup $(BUILDDIR)

install: $(BUILDDIR)/build.ninja
	meson install -C $(BUILDDIR)

check: all $(BUILDDIR)/build.ninja
	meson test -C $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)
