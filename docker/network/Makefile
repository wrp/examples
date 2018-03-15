
# Build a small lan with 4 containers, attach to 1.
# Relies on existence of mydebian from ../debian
#
RHOME=/home/$(USER)
all:
	-docker network create test-network
	#docker network inspect test-network
	-for i in 1 2 3 4; do docker run -dit -v $$HOME/examples:$(RHOME)/examples \
		--name deb$${i} --network test-network mydebian bash \
		|| docker start deb$${i}; \
		docker exec --user $(USER) deb$${i} mkdir $(RHOME)/.ssh; \
		docker exec deb$${i} service ssh start; \
		docker exec --user $(USER) deb$${i} cp $(RHOME)/examples/docker/network/authorized_keys \
			$(RHOME)/.ssh/; \
	done

	docker exec --user $(USER) deb1 cp $(RHOME)/examples/docker/network/id_rsa_deb1 \
		$(RHOME)/.ssh/id_rsa
	@echo 'attaching to container 1'
	-docker attach deb1

clean:
	docker container stop deb{1,2,3,4} || true
	docker container rm deb{1,2,3,4} || true
	docker network rm test-network || true
