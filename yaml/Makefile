
.PHONY: all anchor

list:
	@python -c 'import yaml; import sys; print list(yaml.load_all(sys.stdin))[0:]' < list.yaml

anchor:
	@python -c 'import yaml; import sys; print yaml.load(sys.stdin)' < anchors.yaml

all:
	@python -c 'import yaml; import sys; print yaml.load(sys.stdin)' < example.yaml
