cmake_minimum_required(VERSION 3.14)
project(STM32-Avionics)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(utils)
add_subdirectory(system)
add_subdirectory(devices)
add_subdirectory(desktop_ui)
add_subdirectory(desktop_sim)
