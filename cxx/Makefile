
ifdef DOCKER

CXXFLAGS ?= -fno-elide-constructors -std=c++20

else

all:
	docker run -w /src -e DOCKER=gcc \
		-v $$(pwd):/src \
		--name gcc-examples \
		--user $$(id -u):$$(id -g) -it gcc

endif
