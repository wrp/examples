

all: build run
run:
	docker run vartest

build:
	docker build --build-arg foo='text     with spaces' --build-arg v=9  -t vartest .


.PHONY: build run
