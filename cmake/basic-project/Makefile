
version = $(shell git describe --tags HEAD --match 'v*' | cut -d- -f1,2 | tr - . | sed -e 's/^v//')
.PHONY: all
all: hello

CMakeLists.txt: CMakeLists.txt.in
	sed -e "s/@PKG_VERSION@/$(version)/g" '$^' > $@

hello: CMakeLists.txt src/hello.c include/config.h.in build
	cd build; cmake ..; make; mv hello ..

build:
	mkdir -p build
