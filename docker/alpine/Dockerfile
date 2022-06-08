
FROM alpine
ARG user=docker_user

RUN printf 'set editing-mode vi\nset keymap vi\n' > /etc/inputrc
RUN printf 'bind -v\n' > /root/.editrc
RUN apk add perl-utils
RUN apk add sudo

RUN printf "root ALL=(ALL) ALL\n" > /etc/sudoers
RUN printf "$user ALL=(ALL) NOPASSWD:ALL\n" >> /etc/sudoers
RUN printf '\nDefaults env_keep += "SSH_CLIENT SSH_USER"\n' >> /etc/sudoers

RUN /bin/sh -c 'for i in 1 2; do echo this-is-a-silly-password; done | adduser $user || :'
ENV USER=$user
CMD sudo DOCKER=alpine -u "$USER" /bin/sh

EXPOSE 8125
