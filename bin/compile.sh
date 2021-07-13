#!/bin/bash

# Compile script
# Ensure the docker container has 
# been constructed
echo "Building compilation containter"
docker build -t snek-compile . >/dev/null
echo "Sucessfully built container"

echo "Compiling ELF"
docker run -w /os -v "$(pwd):/os" \
    snek-compile ./bin/internal_compile.sh
echo "Successfully compiled ELF"
