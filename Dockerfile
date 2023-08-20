FROM ubuntu:latest

# Install necessary tools and libraries
RUN apt-get update && apt-get install -y g++ cmake libboost-all-dev

# Copy your source code into the container
COPY . /app
WORKDIR /app

# Build the application
RUN cmake . && make

# Run the application
CMD ["./mystery_mate"]
