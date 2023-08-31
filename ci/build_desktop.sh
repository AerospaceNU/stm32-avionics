#!/bin/bash

touch CMakeListsPrivate.txt

pushd .
mkdir build
cd build
cmake .. -DPROJECT_BUILD_TYPE=desktop -DCMAKE_BUILD_TYPE=Debug

cmake --build . --target desktop_sim desktop_sim_test -- -j -l
if [ $? -ne 0 ];
then
    exit -1
fi

popd
./build/desktop_projects/desktop_sim/desktop_sim_test
if [ $? -ne 0 ];
then
    exit -1
fi
