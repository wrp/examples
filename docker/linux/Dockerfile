
# My minimal linux image
ARG base
FROM $base
RUN printf 'set editing-mode vi\nset keymap vi\n' > /etc/inputrc
RUN printf 'bind -v\n' > /root/.editrc

ARG user=docker_user

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
