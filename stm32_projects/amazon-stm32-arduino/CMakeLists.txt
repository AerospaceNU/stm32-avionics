cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE ${stm32-cmake_SOURCE_DIR}/cmake/stm32_gcc.cmake)

project(amazon-stm32-arduino C ASM)

stm32_fetch_cmsis(F4)
stm32_fetch_hal(F4)

list(APPEND CMAKE_MODULE_PATH ${stm32-cmake_SOURCE_DIR}/cmake)
message("${CMAKE_MODULE_PATH}")

find_package(CMSIS COMPONENTS STM32F401CC REQUIRED)
find_package(HAL COMPONENTS STM32F4 REQUIRED)

set(PROJECT_SOURCES
    main.c
)

add_executable(amazon-stm32-arduino ${PROJECT_SOURCES})
target_link_libraries(amazon-stm32-arduino CMSIS::STM32::F401CC STM32::NoSys)
stm32_print_size_of_target(amazon-stm32-arduino)
