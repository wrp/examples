
# My minimal linux image
ARG base
FROM $base
RUN printf 'set editing-mode vi\nset keymap vi\n' > /etc/inputrc
RUN printf 'bind -v\n' > /root/.editrc

ARG user=docker_user
ARG install_script=/usr/local/bin/my-install
RUN printf '#!/bin/sh\n\n' > ${install_script}; \
if apt-get --version; then \
	apt-get update; \
	apt-get -y install aptitude; \
	printf 'aptitude install -y "$@"\n' >&3; \
elif yum --version; then \
	printf 'yum install -y "$@"\n' >&3; \
elif apk --version; then \
	printf 'apk add "$@"\n' >&3; \
fi 3> ${install_script}
RUN printf 'exit 0' >> ${install_script}
RUN chmod +x ${install_script}

RUN my-install sudo
RUN my-install less
RUN my-install vim
RUN my-install perl
RUN my-install gcc
RUN my-install make
RUN my-install nmap
RUN my-install gdb
RUN my-install git
RUN my-install golang
RUN my-install manpages
RUN my-install man-db
RUN my-install cmake

RUN mkdir -p /etc/skel
COPY bashrc /etc/skel/.bashrc
COPY gitconfig /etc/skel/.gitconfig
COPY vimrc /etc/skel/.vimrc

RUN useradd ${user} || adduser -D ${user}
RUN printf "root ALL=(ALL) ALL\n" > /etc/sudoers
RUN printf "${user} ALL=(ALL) NOPASSWD:ALL\n" >> /etc/sudoers
RUN printf '\nDefaults env_keep += "SSH_CLIENT SSH_USER"\n' >> /etc/sudoers

ENV DOCKER=${base}-${user}

CMD /bin/sh
