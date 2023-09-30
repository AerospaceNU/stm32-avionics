set(PROJECT hello_usb_cpp)

cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(${PROJECT} C CXX ASM)

pico_sdk_init()

add_executable(${PROJECT} main.cpp)

target_link_libraries(${PROJECT} pico_stdlib)

pico_add_extra_outputs(${PROJECT})
pico_enable_stdio_usb(${PROJECT} 1)
pico_enable_stdio_uart(${PROJECT} 0)
