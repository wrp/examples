CXXFLAGS ?= -fno-elide-constructors -std=c++20

source = $(shell ls *.cc)
exe = $(source:.cc=)

all: $(exe)



ifndef DOCKER
docker:
	docker run -w /src -e DOCKER=gcc \
		-v $$(pwd):/src \
		--user $$(id -u):$$(id -g) -it gcc
endif

clean:
	git clean -xdff
