
.PHONY: debian alpine build
build:
	docker build -f dockerfile-basics --build-arg var1=foo -t dockerfile-basics .
	docker run dockerfile-basics


debian alpine:
	cd $@ && make
