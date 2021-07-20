#!/bin/bash

# Create build directory
BUILD_DIR=build
OBJ_DIR=$BUILD_DIR/obj
BIN_DIR=$BUILD_DIR/bin
TMP_DIR=$BUILD_DIR/tmp

COMPILER=/usr/local/x86_64elfgcc/bin/x86_64-elf-gcc
LINKER=/usr/local/x86_64elfgcc/x86_64-elf/bin/ld

if [[ -d $BUILD_DIR ]]; then
    rm -r $BUILD_DIR
fi
mkdir $BUILD_DIR
mkdir $OBJ_DIR
mkdir $BIN_DIR
mkdir $TMP_DIR

# Compile bootloader with NASM
nasm src/bootloader/bootloader.asm -f bin -o $BIN_DIR/bootloader.bin
nasm src/bootloader/protected_mode.asm -f elf64 -o $OBJ_DIR/protected_mode.o

# Compile with cross compiler
$COMPILER -ffreestanding -mno-red-zone -m64 -c src/kernel/entry.c -o $OBJ_DIR/entry.o

# Link with custom linker
$LINKER -o $TMP_DIR/kernel.tmp -Ttext 0x9000 \
    $OBJ_DIR/protected_mode.o \
    $OBJ_DIR/entry.o

objcopy -O binary $TMP_DIR/kernel.tmp $BIN_DIR/kernel.bin

# Create ELF
cat $BIN_DIR/bootloader.bin \
    $BIN_DIR/kernel.bin \
    > $BUILD_DIR/image.elf

echo "Internal compilation successful"
