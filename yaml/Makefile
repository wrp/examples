
files = $(shell ls *.yaml | sort)


.PHONY: default
default: $(files:.yaml=)

%: %.yaml
	@echo; echo $<
	@python -c 'import yaml; import sys; print list(yaml.load_all(sys.stdin))[0:]' < $<
