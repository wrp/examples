base = $(shell basename $$(pwd))
package_name = example/${USER}/$(base)

.PHONY: all install
all: go.mod
	go run .

$(base):
	go build .

go.mod:
	go mod init $(package_name)

install: go.mod
	@ go install -v
