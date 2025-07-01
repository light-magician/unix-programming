FROM rust:1.77-slim

RUN apt-get update && apt-get install -y \
    clang \
    build-essential \
    gdb \
    lldb \
    vim \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
COPY . /workspace
