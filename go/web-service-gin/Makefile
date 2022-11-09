

.PHONY: all check run

all: run

run:
	go run main.go

go.mod:
	go mod init example.com/m/v2

%: %.go
	go build $^

check:
	go test
