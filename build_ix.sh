#! /bin/sh

mkdir ./build
pushd ./build
cmake ..
cmake --build .
popd
