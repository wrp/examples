# Make both images without nice names
all:
	docker build .

base:
	docker build --target=base . -t local/base

second:
	docker build --target=second-image . -t local/second
