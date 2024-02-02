FROM ubuntu:22.04
WORKDIR /dir
COPY Directory Directory
RUN chmod +x Directory
RUN apt-get update && apt-get install -y libsfml-dev
ENTRYPOINT [ "./Directory"]
EXPOSE 8201