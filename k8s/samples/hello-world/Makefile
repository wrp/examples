
all: kustomization.yaml
	kubectl apply -k .

%.yaml: %.yaml.in
	m4 $< > $@
