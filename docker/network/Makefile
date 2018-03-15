
# Build a small lan with 4 containers, attach to 1.
# Relies on existence of mydebian from ../debian
all:
	-docker network create test-network
	-docker network inspect test-network
	-for i in 1 2 3 4; do docker run -dit --name deb$${i} --network test-network mydebian bash; done
	@echo 'attaching to container 1'
	-docker attach deb1

clean:
	docker container stop deb{1,2,3,4} || true
	docker container rm deb{1,2,3,4} || true
	docker network rm test-network || true
