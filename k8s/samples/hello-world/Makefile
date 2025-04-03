
all: kustomization.yaml base/deploy.yaml
	kubectl apply -k .

%.yaml: %.yaml.in
	m4 $< > $@
