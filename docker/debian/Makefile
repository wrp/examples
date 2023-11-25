
docker_file ?= Dockerfile-$(USER)
image ?= debian-$(USER)-dev
image: Dockerfile-$(USER)

include ../common-makefile

Dockerfile-$(USER): Dockerfile
	sed -e 's/@user@/$(USER)/g' $< > $@
