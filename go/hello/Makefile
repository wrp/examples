package_name = example/user/hello


.PHONY: all install
all:
	go run hello.go

hello:
	go build hello.go


go.mod:
	go mod init $(package_name)

install: go.mod
	@ go install
	@ echo $(package_name) installed in $${GOBIN-$${GOPATH-$$HOME/go}/bin}
	go list -f '{{.Target}}'
