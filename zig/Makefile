# Note: you may need to first go to ../docker/debian
# and run "make image"

image ?= zig-playground
docker_file ?= Dockerfile-$(USER)
include ../docker/common-makefile
image: Dockerfile-$(USER)
