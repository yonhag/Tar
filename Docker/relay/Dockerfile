FROM ubuntu:22.04
WORKDIR /relay
COPY Relay Relay
RUN chmod +x Relay
RUN apt-get update && apt-get install -y libsfml-dev
ENTRYPOINT [ "./Relay"]
EXPOSE 8200