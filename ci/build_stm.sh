#!/bin/bash

touch CMakeListsPrivate.txt

# This assumes arm-none-eabi-* is on your PATH
pushd .
mkdir build_stm
cd build_stm
cmake .. -DPROJECT_BUILD_TYPE=stm32 -DCMAKE_TOOLCHAIN_FILE="../cmake/stm32-cmake/cmake/stm32_gcc.cmake" -DCMAKE_BUILD_TYPE=Debug

cmake --build . --target fcb_v0 fcb_v1 fcb_v2 groundstation amazon-stm32-arduino prop_adcboard_v2 -- -j -l
if [ $? -ne 0 ];
then
    exit -1
fi
