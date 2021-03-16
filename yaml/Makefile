
files = $(shell ls *.yaml | sort)


.PHONY: default
default: $(files:.yaml=)

%: %.yaml
	@python -c 'import yaml; import json; import sys; print json.dumps(list(yaml.load_all(sys.stdin, Loader=yaml.FullLoader))[0:])' < $<
