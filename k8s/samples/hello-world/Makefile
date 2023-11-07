
all: kustomization.yaml deploy.yaml
	kubectl apply -k .

%.yaml: %.yaml.in
	m4 $< > $@
