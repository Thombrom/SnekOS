#!/bin/bash

# Create build directory
BUILD_DIR=build
OBJ_DIR=$BUILD_DIR/obj
BIN_DIR=$BUILD_DIR/bin
TMP_DIR=$BUILD_DIR/tmp

LINKER=/usr/local/x86_64elfgcc/x86_64-elf/bin/ld
COMPILER=/usr/local/x86_64elfgcc/bin/x86_64-elf-gcc

COMPILER_PARAM="-Ttext 0x9000 -ffreestanding -mno-red-zone -m64"

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
mkdir $OBJ_DIR/kernel
$COMPILER $COMPILER_PARAM -c src/kernel/entry.c -o $OBJ_DIR/kernel/entry.o
$COMPILER $COMPILER_PARAM -c src/kernel/io.c -o $OBJ_DIR/kernel/io.o

mkdir $OBJ_DIR/driver
$COMPILER $COMPILER_PARAM -c src/driver/screen_vga.c -o $OBJ_DIR/driver/screen_vga.o
$COMPILER $COMPILER_PARAM -c src/driver/serial.c -o $OBJ_DIR/driver/serial.o

# Link with custom linker
$LINKER -T"bin/link.ld"

# Create ELF
cat $BIN_DIR/bootloader.bin \
    $BIN_DIR/kernel.bin \
    > $BUILD_DIR/image.elf

echo "Internal compilation successful"
