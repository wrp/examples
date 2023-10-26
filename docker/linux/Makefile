
# invoke with base defined.  eg 'make base=alpine image'
base ?= debian
image = $(base)-$(USER)

include ../common-makefile

all:
	make base=alpine image
	make base=fedora image
	make base=debian image
