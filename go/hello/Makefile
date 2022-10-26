package_name = example/user/hello


.PHONY: all install test
all:
	go run hello.go foo.go

hello:
	go build hello.go foo.go

test: go.mod
	go test

go.mod:
	go mod init $(package_name)

install: go.mod
	@ go install
	@ echo $(package_name) installed in $${GOBIN-$${GOPATH-$$HOME/go}/bin}
	go list -f '{{.Target}}'
