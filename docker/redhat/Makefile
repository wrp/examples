

all: build run
build:
	docker build -t myfedora .

run:
	# Map port 8130 on the local host to port 8125 on the container
	docker run -it -p 8125 -v $$HOME:/home/william myfedora

.PHONY: all build run
