cmake_minimum_required(VERSION 3.14)
project(STM32-Avionics)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(desktop_devices)
add_subdirectory(desktop_sim)
add_subdirectory(pybind_test)
