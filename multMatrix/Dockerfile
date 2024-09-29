FROM ubuntu:20.04
RUN apt-get update
RUN apt-get install -y software-properties-common
RUN apt-get update

EXPOSE 15000

COPY source/server /server
RUN chmod +x /server

CMD /server
