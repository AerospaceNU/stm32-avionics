cmake_minimum_required(VERSION 3.14)
project(STM32-Avionics)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(Utils)
add_subdirectory(System)
add_subdirectory(Devices)
add_subdirectory(desktop-simulator)
