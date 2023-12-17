# Use the official image as a parent image
#FROM ubuntu:latest
FROM mystery-mate:latest

# Update package list, install dependencies and LLDB
RUN apt-get update && \
    apt-get install -y \
    cmake \
    pkg-config \
    g++ \
    gdb \
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
COPY . /build
WORKDIR /build

# Build your project with debug symbols
RUN cmake -DCMAKE_BUILD_TYPE=Debug . && make

EXPOSE 8080

CMD ["/bin/bash"]
#CMD ["./ChessProject"]
