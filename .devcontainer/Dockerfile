# Entorno de desarrollo del microservicio. Ubuntu 24.04 = mismo sistema y GCC 13
# que el runner ubuntu-latest de GitHub Actions.
FROM mcr.microsoft.com/devcontainers/base:ubuntu-24.04

ARG CLANG_FORMAT_VERSION=21.1.8

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
       build-essential cmake ninja-build gdb pipx \
    && rm -rf /var/lib/apt/lists/*

# clang-format fijado a la misma versión que el hook de pre-commit y el CI
RUN PIPX_HOME=/opt/pipx PIPX_BIN_DIR=/usr/local/bin \
    pipx install "clang-format==${CLANG_FORMAT_VERSION}"
