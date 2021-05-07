
files = $(shell ls *.yaml | sort)


.PHONY: default
default: $(files:.yaml=)

%: %.yaml
	@python3 -c $$'import yaml; import json; import sys;\n'$$( : \
	)'for x in list(yaml.load_all(sys.stdin, Loader=yaml.SafeLoader)): print(json.dumps(x))' < $<
