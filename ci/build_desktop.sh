#!/bin/bash

mkdir build
cd build
cmake ..

cmake --build .
if [ $? -ne 0 ];
then
    exit -1
fi
