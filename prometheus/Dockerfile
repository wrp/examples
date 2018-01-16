
FROM debian
RUN printf 'set editing-mode vi\nset keymap vi\n' > /etc/inputrc
RUN apt-get update
RUN apt update
RUN apt-get install -y nmap
RUN apt-get install -y collectd collectd-utils

RUN printf 'deb http://deb.debian.org/debian sid main\n' > /etc/apt/sources.list
RUN apt-get update
RUN apt-get install -y prometheus=2.0.0+ds3-1

RUN rm /etc/prometheus/prometheus.yml /etc/collectd/collectd.conf
COPY prometheus.yml /etc/prometheus
COPY collectd.conf /etc/collectd
COPY prometheus.wrapper /usr/bin
COPY generate_stats /usr/bin

CMD /usr/bin/prometheus.wrapper

EXPOSE 8124
