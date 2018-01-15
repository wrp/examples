metrics_path=/var/lib/prometheus/metrics2
host_path=$$HOME/docker$(metrics_path)

all: build run

build:
	docker build -t myprometheus .

run:
	docker run -it -p 8124 -v $(host_path):$(metrics_path) myprometheus
