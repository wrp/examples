base = $(shell basename $$(pwd))
package_name = example/${USER}/$(base)

.PHONY: all install
all: go.mod input
	go run main.go input

$(base):
	go build .

go.mod:
	go mod init $(package_name)

install: go.mod
	@ go install -v


input:
	@ echo A bunch of text > $@
