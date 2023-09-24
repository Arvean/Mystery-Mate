FROM ubuntu:latest

# Update package list and install dependencies
RUN apt-get update && \
    apt-get install -y \
    cmake \
    pkg-config \
    g++ \
    libboost-all-dev \
    libgtest-dev \
    libgmock-dev \
    uuid-dev \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

# Build Google Test
WORKDIR /usr/src/googletest/googletest
RUN cmake . && make && cp lib/*.a /usr/lib

# Build Google Mock
WORKDIR /usr/src/googletest/googlemock
RUN cmake . && make && cp lib/*.a /usr/lib

# Copy your project's source code to the container
COPY . /my_project
WORKDIR /my_project

# Build your project
RUN cmake . && make
