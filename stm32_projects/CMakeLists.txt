
set(STM32_CMAKE_DIR ${CMAKE_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${STM32_CMAKE_DIR})

add_subdirectory(device_drivers)
add_subdirectory(fcb_v0)
add_subdirectory(fcb_v1)
add_subdirectory(fcb_v2)
add_subdirectory(groundstation)
