

.PHONY: all check

all: map

%: %.go
	go build -gcflags=-m $^

check:
	go test
