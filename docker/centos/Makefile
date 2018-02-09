

all: build run
build:
	docker build -t mycentos .

run:
	docker run -it -p 8125 -v $$HOME:/home/william myfedora

.PHONY: all build run
