# SnekO
A small operating system to play snake

The operating system boots and launches a snake program which can be played with the arrow keys.

### Dependencies to build:
 - Docker

To build the image, run bin/compile.sh. It will create a docker image with a cross compiler which will subsequently build the image. The final image can be found as build/image.elf.

### Emulation

I recommend using bochs. The repo has a bochsrc file already in the root.
