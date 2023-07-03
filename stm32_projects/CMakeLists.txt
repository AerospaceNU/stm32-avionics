
set(STM32_CMAKE_DIR ${CMAKE_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${STM32_CMAKE_DIR})

find_package(Git)
set(GENERATED_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/)
add_custom_target(stm_version
  ${CMAKE_COMMAND} -D SRC=${CMAKE_SOURCE_DIR}/desktop_projects/desktop_sim/generate/git_info.h.in
                   -D DST=${GENERATED_INCLUDE_DIR}/git_info.h
                   -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
                   -P ${CMAKE_SOURCE_DIR}/cmake/GenerateVersionHeader.cmake
  )

add_subdirectory(device_drivers)
add_subdirectory(fcb_v0)
add_subdirectory(fcb_v1)
add_subdirectory(fcb_v2)
add_subdirectory(groundstation)
