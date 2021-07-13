FROM ubuntu

# Update
RUN apt-get update && \
    apt-get install -y apt-utils build-essential

# Prevent location input
ENV DEBIAN_FRONTEND noninteractive
ENV DEBIAN_NONINTERACTIVE_SEEN true

RUN echo "tzdata tzdata/Areas select Europe" > /tmp/preseed.txt; \
    echo "tzdata tzdata/Zones/Europe select Berlin" >> /tmp/preseed.txt; \
    debconf-set-selections /tmp/preseed.txt && \
    apt-get update && \
    apt-get install -y tzdata

# Install dependencies
RUN apt-get install -y nasm \
    curl \
    bison \
    flex \
    libgmp3-dev \
    libmpc-dev \
    libmpfr-dev \
    texinfo

ENV PREFIX=/usr/local/x86_64elfgcc
ENV TARGET=x86_64-elf
ENV PATH=$PREFIX/bin:$PATH

# Install binutils
RUN mkdir /tmp/src && \
    cd /tmp/src && \
    curl -O http://ftp.gnu.org/gnu/binutils/binutils-2.35.1.tar.gz && \
    tar xf binutils-2.35.1.tar.gz && \
    mkdir binutils-build && \
    cd binutils-build && \
    ../binutils-2.35.1/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$PREFIX 2>&1 | tee configure.log && \
    make all install 2>&1 | tee make.log

# Install GCC
RUN cd /tmp/src && \
    curl -O https://ftp.gnu.org/gnu/gcc/gcc-10.2.0/gcc-10.2.0.tar.gz && \
    tar xf gcc-10.2.0.tar.gz && \
    mkdir gcc-build && \
    cd gcc-build && \
    ../gcc-10.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-libssp --enable-languages=c++ --without-headers && \
    make all-gcc && \
    make all-target-libgcc && \
    make install-gcc && \
    make install-target-libgcc

ENTRYPOINT ["/bin/bash", "-c"]

