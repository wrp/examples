

all: build run
build:
	docker build -t mydebian .

run:
	@# -p 8130:8125 Map port 8130 on the local host to port 8125 on the container
	@# -p 8125      Let docker select the port.  Discover with 'docker port $container'
	-docker run -it -p 8125 -v $$HOME:/home/williamp mydebian

	@# To access that port, the container needs to use its ip address which
	@# can be found with 'docker inspect $container_id | grep IPAddress'
	@# eg, on the container, run 'ncat -kl $this_ip 8125'
	@# The IP is also found in /etc/hosts within the container
	@#
	# EXITED the debian docker container
clean:
	docker system prune -a -f

.PHONY: all build run clean
