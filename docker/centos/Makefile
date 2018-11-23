

NAME=mycentos
all: build run
build:
	docker build -t $(NAME) .

run:
	docker run -it -p 8125 -v $$HOME:/home/docker_user $(NAME)

.PHONY: all build run
