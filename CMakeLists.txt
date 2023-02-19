cmake_minimum_required(VERSION 3.14)
project(STM32-Avionics)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(allwpilib)
add_subdirectory(utils)
add_subdirectory(system)
add_subdirectory(devices)
add_subdirectory(desktop_sim)
