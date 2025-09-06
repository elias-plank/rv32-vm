#!/bin/sh

mkdir -p ./build
pushd build
cmake ..
cmake --build .
popd
./build/rv32_vm
