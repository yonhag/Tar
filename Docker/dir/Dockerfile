FROM ubuntu:22.04

# Set the working directory
WORKDIR /dir

# Copy the necessary files
COPY Directory Directory

# Ensure the directory has executable permissions
RUN chmod +x Directory

# Update the package list and install necessary packages
RUN apt-get update && apt-get install -y libsfml-dev rng-tools

# Ensure rng-tools runs at startup
CMD rngd -r /dev/urandom && ./Directory

# Expose the necessary port
EXPOSE 8201
