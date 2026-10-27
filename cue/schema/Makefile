
.PHONY: all dev prod staging
all:
	cue cmd gen  # Generate yaml files as specified in gen_tool.cue

vet:
	cue vet schema.cue data.yaml -d '#User'

.SECONDEXPANSION:
dev prod staging: $$(addsuffix .yaml,$$@)

dev.yaml prod.yaml staging.yaml: config.cue
	cue cmd -t env=$(basename $@) gen
