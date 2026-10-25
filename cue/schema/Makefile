
.PHONY: all dev prod staging
all:
	cue cmd gen  # Generate yaml files as specified in gen_tool.cue

.SECONDEXPANSION:
dev prod staging: $$(addsuffix .yaml,$$@)

dev.yaml prod.yaml staging.yaml:
	cue cmd -t env=$(basename $@) gen
