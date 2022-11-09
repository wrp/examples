

.PHONY: all check run

all: run

run:
	go run http.go

%: %.go
	go build $^

check:
	go test
