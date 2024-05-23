FROM ubuntu:22.04
WORKDIR /relay
COPY Relay Relay
COPY dirlist.txt dirlist.txt
RUN chmod +x Relay
RUN apt-get update && apt-get install -y libsfml-dev gdbserver
EXPOSE 1234
ENTRYPOINT ["gdbserver", ":1234", "./Relay"]