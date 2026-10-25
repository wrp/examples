
all:
	cue cmd gen  # Generate yaml files as specified in gen_tool.cue

dev prod staging:
	cue cmd -t env=$@ gen
