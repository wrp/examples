base = $(shell basename $$(pwd))
package_name = example/$(base)

.PHONY: all install
all: go.mod
	go run . $(ARGS)

$(base):
	go build .

go.mod:
	go mod init $(package_name)

install: go.mod
	# See `go help install` for details.  By default, executbale
	# will be installed in $$GOBIN or $$GOPATH/bin or $$HOME/go/bin
	@ go install -v

check:
	for i in \
		map \
	; do cd "$$i" && go test; done
