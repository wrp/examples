
# invoke with base defined.  eg 'make base=alpine'
base ?= debian
image = $(base)-$(USER)

all:
	make base=alpine image
	make base=fedora image
	make base=debian image

include ../common-makefile
