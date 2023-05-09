


SRCS = $(wildcard *.go)
EXE = $(patsubst %.go,%,$(SRCS))

.PHONY: all check

all: $(EXE)

%: %.go
	go build -gcflags=-m $^

check:
	go test
