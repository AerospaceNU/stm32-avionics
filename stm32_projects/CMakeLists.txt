
include(FetchContent)
FetchContent_Declare(
  stm32-cmake
  GIT_REPOSITORY https://github.com/ObKo/stm32-cmake
)
FetchContent_MakeAvailable(stm32-cmake)

include(${stm32-cmake_SOURCE_DIR}/cmake/stm32/utilities.cmake)
include(${stm32-cmake_SOURCE_DIR}/cmake/stm32/common.cmake)

# include()

add_subdirectory(amazon-stm32-arduino)
