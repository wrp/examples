
FROM debian
RUN printf 'set editing-mode vi\nset keymap vi\n' > /etc/inputrc
RUN printf 'bind -v\n' > /root/.editrc
RUN apt-get update
RUN apt-get -y install aptitude
RUN aptitude install -y sudo
RUN aptitude install -y less
RUN aptitude install -y vim
RUN aptitude install -y perl
RUN aptitude install -y gcc
RUN aptitude install -y make
RUN aptitude install -y nmap
RUN apt-get update && aptitude install -y gdb
RUN aptitude install -y git
RUN aptitude install -y libncurses-dev
RUN aptitude install -y ncurses-doc
RUN aptitude install -y golang
RUN aptitude install -y manpages
RUN aptitude install -y man-db
ARG user=docker_user
ARG uid=1000
RUN adduser --uid $uid --quiet $user
ENV USER=$user
RUN printf "root ALL=(ALL) ALL\n" > /etc/sudoers
RUN printf "$user ALL=(ALL) NOPASSWD:ALL\n" >> /etc/sudoers
run mkdir /git-bug
copy Dockerfile git-bug*.bundle /git-bug
run chown -R $user /git-bug
USER $user
run if test -f /git-bug/git-bug*.bundle; then \
	cd /git-bug && git init && git fetch git-bug*.bundle \
	&& git checkout -b main FETCH_HEAD; \
	else \
		git clone https://github.com/MichaelMure/git-bug.git; \
	fi
workdir /git-bug
RUN make install
copy bashrc /home/$user/.bashrc
CMD sudo DOCKER=debian -u "$USER" bash
