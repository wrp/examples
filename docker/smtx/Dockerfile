
FROM debian
RUN printf 'set editing-mode vi\nset keymap vi\n' > /etc/inputrc
RUN printf 'bind -v\n' > /root/.editrc
RUN apt-get update
RUN apt-get -y install aptitude
RUN aptitude install -y sudo
RUN aptitude install -y less
RUN aptitude install -y manpages
RUN aptitude install -y man-db
RUN aptitude install -y vim
RUN aptitude install -y perl
RUN aptitude install -y gcc
RUN aptitude install -y procps
RUN aptitude install -y make
RUN aptitude install -y dtach
RUN aptitude install -y tmux
RUN aptitude install -y jq
RUN aptitude install -y libncurses6
RUN aptitude install -y libncurses-dev
RUN aptitude install -y ncurses-doc
COPY smtx-47c3eaf.tar.gz /usr/local/src
WORKDIR /usr/local/src
RUN tar -xzf smtx-*.tar.gz
RUN ln -s smtx-??????? smtx
WORKDIR /usr/local/src/smtx
RUN tic smtx.ti
RUN ./configure CFLAGS='--coverage -g -O0'
RUN make
RUN make install
