
image ?= mydebian

all: build run
build:
	docker build --build-arg user=$(USER) -t $(image) .

run:
	@# -p 8130:8125 Map port 8130 on the local host to port 8125 on the container
	@# -p 8125      Let docker select the port.  Discover with 'docker port $container'
	@# ($container can be found from 'docker ps')
	@# -v localmnt:mount_in_docker_image
	-rename-tmux-window debian

	@if id=$$(docker ps --filter label=$(USER)-debian -q | grep .); then \
		echo "Attaching to docker containter $$id"; \
		docker exec -it -e DOCKER=debian -u $(USER) "$$id" bash; \
	else \
		echo 'Creating new docker container'; \
		docker run -it \
		--privileged \
		-p 8125 \
		-v $(HOME):/home/$(USER) \
		-e SAMPLE_ENV=/$(USER) \
		--label $(USER)-debian \
		--cap-add=SYS_PTRACE --security-opt seccomp=unconfined $(image); \
	fi
	@# To access that port, the container needs to use its ip address which
	@# can be found with 'docker inspect $container_id | grep IPAddress'
	@# eg, on the container, run 'ncat -kl $this_ip 8125'
	@# The IP is also found in /etc/hosts within the container
	@#
	# EXITED the debian docker container
clean:
	docker system prune -a -f

.PHONY: all build run clean
