
# My minimal linux image
ARG base
ARG uid=1000
FROM $base
RUN printf 'set editing-mode vi\nset keymap vi\n' > /etc/inputrc
RUN printf 'bind -v\n' > /root/.editrc

RUN printf '#!/bin/sh\n\n' > /usr/local/bin/my-install; \
if apt-get --version; then \
	apt-get update; \
	apt-get -y install aptitude; \
	printf 'aptitude install -y "$@"\n' >&3; \
elif yum --version; then \
	printf 'yum install -y "$@"\n' >&3; \
elif apk --version; then \
	printf 'apk add "$@"\n' >&3; \
fi 3> /usr/local/bin/my-install
RUN chmod +x /usr/local/bin/my-install

RUN my-install sudo
RUN my-install less
RUN my-install vim
RUN my-install perl
RUN my-install gcc
RUN my-install make
RUN my-install nmap
RUN my-install gdb
RUN my-install git
# RUN my-install golang
# RUN my-install manpages
# RUN my-install man-db
RUN my-install cmake
RUN mkdir -p /etc/skel

COPY bashrc /etc/skel/.bashrc
COPY gitconfig /etc/skel/.gitconfig
COPY vimrc /etc/skel/.vimrc

ENV DOCKER=${base}-${user}

CMD /bin/bash
