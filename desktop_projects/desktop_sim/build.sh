#!/usr/bin/env bash

# If we're in desktop-simulator, back up a directory
if [[ $(basename $PWD) == "desktop_sim" ]];
then
  cd ../
fi

# Ensure we're in stm32-avionics directory
if ! [[ $(basename $PWD) == "stm32-avionics" ]];
then
  echo "Not in stm32-avionics directory! Exiting build"
  exit
fi

# Make build directory if it doesn't exist and enter it
mkdir -p build
cd build

# Set compiler variables
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++

# Build project
if [ $OSTYPE == 'msys' ];
then
  wsl cmake ..
  wsl cmake --build .
else
  cmake ..
  cmake --build .
fi
