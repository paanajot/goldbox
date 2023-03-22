############################################################
### config
ARG GTEST_VERSION=release-1.11.0
ARG CLANG_VERSION=14
ARG GCC_VERSION=12
ARG CPPCHECK_VERSION=2.8
ARG CMAKELANG_VERSION=0.6.13

### multi-stage builds
### install and configure toolchain
FROM ubuntu:22.04 AS base

# You use this mode when you need zero interaction while installing or upgrading the system via apt.
ARG DEBIAN_FRONTEND=noninteractive
ARG GCC_VERSION
ARG CLANG_VERSION

ENV CXX=/usr/bin/g++-${GCC_VERSION}
ENV PATH="${PATH}:/usr/lib/llvm-${CLANG_VERSION}/bin/"
WORKDIR /workdir

### basic packages
# Building gcc from sources takes a long time, therefore I decided to install it from system packages
RUN apt-get update && apt-get install --no-install-recommends \
    cmake g++-${GCC_VERSION} git build-essential ca-certificates -y && \
    rm -rf /var/lib/apt/lists/*

### install llvm (clang-tidy, clang-format, etc.)
RUN apt-get update && apt-get install --no-install-recommends \
    wget gpg-agent lsb-release software-properties-common -y && \
    wget https://apt.llvm.org/llvm.sh && \
    chmod +x llvm.sh && \
    ./llvm.sh ${CLANG_VERSION} && \
    apt-get install --no-install-recommends \
    libclang-${CLANG_VERSION}-dev clang-tidy-${CLANG_VERSION} clang-format-${CLANG_VERSION} -y && \
    rm -rf /var/lib/apt/lists/*

### cmakelang - cmake-format, cmake-lint ...
ARG CMAKELANG_VERSION
RUN apt-get update && apt-get install --no-install-recommends \
    python3-pip -y && \
    pip install --no-cache-dir cmakelang==${CMAKELANG_VERSION} && \
    rm -rf /var/lib/apt/lists/*

### include what you use tool
FROM base AS iwyu
ARG CLANG_VERSION
RUN mkdir /iwyu && \
    cd /iwyu && \
    git clone https://github.com/include-what-you-use/include-what-you-use.git \
    --single-branch --branch clang_${CLANG_VERSION} . && \
    mkdir build && \
    cd build && \
    cmake -G "Unix Makefiles" -DCMAKE_PREFIX_PATH=/usr/lib/llvm-${CLANG_VERSION} .. && \
    make

### cppcheck
FROM base AS cppcheck
ARG CPPCHECK_VERSION
RUN cd /workdir && \
    git clone https://github.com/danmar/cppcheck.git --single-branch --branch ${CPPCHECK_VERSION} && \
    cd cppcheck && \
    mkdir build && \
    cd build && \
    cmake -DUSE_MATCHCOMPILER=ON .. && \
    cmake --build . && \
    cmake --install . && \
    cd /workdir && \
    rm -rf cppcheck

### google test && google mock
### https://github.com/google/googletest/blob/main/googletest/README.md
FROM base AS googletest
ARG GTEST_VERSION
RUN git clone https://github.com/google/googletest.git --single-branch --branch ${GTEST_VERSION} && \
    cd googletest && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make && \
    make install && \
    cd /workdir && \
    rm -rf googletest

### bloaty - a size profiler for binaries
### '-d compileunits' option doesn't work with g++11, it works with g++9
###  https://github.com/google/bloaty
FROM base AS bloaty
RUN apt-get update && apt-get install --no-install-recommends libz-dev -y
RUN git clone https://github.com/google/bloaty.git --single-branch --recursive && \
    cd bloaty && \
    cmake -B build -S . && \
    cmake --build build --target install -j3

############################################################
### final image
FROM base AS final
COPY --from=cppcheck /usr/local/bin/cppcheck /usr/local/bin/cppcheck
COPY --from=cppcheck /usr/local/share/Cppcheck /usr/local/share/Cppcheck
COPY --from=googletest /usr/local/include/ /usr/local/include/
COPY --from=googletest /usr/local/lib/ /usr/local/lib/
COPY --from=iwyu /iwyu /iwyu
COPY --from=bloaty /usr/local/lib/Bloaty /usr/local/lib/Bloaty
COPY --from=bloaty /usr/local/bin/bloaty /usr/local/bin/bloaty

### install goldbox lib
ADD lib /workdir/
RUN cd /workdir && \
    ./install.sh && \
    rm -rf /workdir

############################################################
### install and configure dev utils
FROM final AS cpp

RUN apt-get update && apt-get install --no-install-recommends \
    sudo vim -y && \
    rm -rf /var/lib/apt/lists/*

### config
ARG UID=1000
ARG USERNAME=dev

RUN useradd -l -m $USERNAME -u $UID

### avoiding VS code extension reinstalls
RUN mkdir -p /home/$USERNAME/.vscode-server/extensions \
    /home/$USERNAME/.vscode-server-insiders/extensions && \
    chown -R $USERNAME \
    /home/$USERNAME/.vscode-server \
    /home/$USERNAME/.vscode-server-insiders

### color gtest output
ENV GTEST_COLOR=1
