#!/bin/bash

set -e

# Clone onut
git submodule update --init

# Create build dir
mkdir -p build_linux

# cd to build dir
cd build_linux

# We want to use hunter and we want to build the stand alone (onut.exe)
cmake -DCMAKE_BUILD_TYPE=Release ..

# Compile
make -j8

cd ../../..
