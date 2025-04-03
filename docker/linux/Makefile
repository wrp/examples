
# invoke with base defined.  eg 'make base=alpine image'
base ?= debian
image = $(base)-$(USER)
docker_file=Dockerfile-$(base)

include ../common-makefile

image: Dockerfile-$(base)

all: Dockerfile-$(base)
	make base=alpine image
	make base=fedora image
	make base=debian image

Dockerfile-$(base): Dockerfile
	sed -e 's/@base@/$(base)/g' $< > $@
