
all:
	cue cmd gen  # Generate yaml files as specified in gen_tool.cue

dev.yaml prod.yaml staging.yaml:
	cue cmd -t env=$(basename $@) gen
