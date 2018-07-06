
.PHONY: default

default: anchors example list variables

%: %.yaml
	echo $<
	@python -c 'import yaml; import sys; print list(yaml.load_all(sys.stdin))[0:]' < $<
