CXXFLAGS ?= -fno-elide-constructors -std=c++20

ifndef DOCKER

all:
	docker run -w /src -e DOCKER=gcc \
		-v $$(pwd):/src \
		--user $$(id -u):$$(id -g) -it gcc
endif

clean:
	git clean -xdff
