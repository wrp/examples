

.PHONY: all check run deps

all: run

run:
	go run main.go

deps:
	go list -m all  # Show all module dependencies

go.mod:
	go mod init example/web-service-gin
	go get .

%: %.go
	go build $^

check:
	go test
