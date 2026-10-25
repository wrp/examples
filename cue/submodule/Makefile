
.PHONY: all registry tidy fix

all: cue-mod
	cue eval .

# export CUE_REGISTRY=localhost:5001/cuemodules
registry:
	cue mod registry localhost:5001

tidy:
	cue mod tidy

cue-mod:
	cue mod init wrp.example/top@v0

fix:
	cue mod fix
