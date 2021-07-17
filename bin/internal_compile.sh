#!/bin/bash

# Create build directory
BUILD_DIR=build
OBJ_DIR=obj

if [[ -d $BUILD_DIR ]]; then
    rm -r $BUILD_DIR
fi
mkdir $BUILD_DIR
mkdir $BUILD_DIR/$OBJ_DIR

# Compile bootloader with NASM

nasm src/bootloader/bootloader.asm -f bin -o $BUILD_DIR/$OBJ_DIR/bootloader.bin
nasm src/bootloader/protected_mode.asm -f bin -o $BUILD_DIR/$OBJ_DIR/protected_mode.bin

# Create ELF
cat $BUILD_DIR/$OBJ_DIR/bootloader.bin \
    $BUILD_DIR/$OBJ_DIR/protected_mode.bin \
    > $BUILD_DIR/image.elf

echo "Internal compilation successful"
