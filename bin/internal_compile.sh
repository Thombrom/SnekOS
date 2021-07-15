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
BOOTLOADER=src/bootloader/bootloader.asm

nasm $BOOTLOADER -f bin -o $BUILD_DIR/$OBJ_DIR/bootloader.bin


# Create ELF
cp $BUILD_DIR/$OBJ_DIR/bootloader.bin $BUILD_DIR/image.elf

echo "Internal compilation successful"
