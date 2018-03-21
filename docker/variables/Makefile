

all: build run
run:
	docker run vartest

build:
	docker build --build-arg foo=banana  -t vartest .


.PHONY: build run
