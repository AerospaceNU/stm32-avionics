set(GENERATED_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/)
find_package(Git)
add_custom_target(version
  ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/generate/git_info.h.in
                   -D DST=${GENERATED_INCLUDE_DIR}/git_info.h
                   -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
                   -P ${CMAKE_SOURCE_DIR}/cmake/GenerateVersionHeader.cmake
  )

set(SIMULATOR_FILES_LIST
        main.cpp 
        desktop_hardware_manager.cpp 
        board_config.c)

add_executable(desktop_sim 
  ${SIMULATOR_FILES_LIST}
  ${DEVICES_FILES_LIST}
  ${UTILS_FILES_LIST}
  ${SYSTEM_FILES_LIST}
)
add_dependencies(desktop_sim version)

target_include_directories(desktop_sim PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${DEVICES_INCLUDE_DIRS}
  ${SYSTEM_INCLUDE_DIRS}
  ${UTILS_INCLUDE_DIRS}
  ${DEVICES_COMMON_INCLUDE_DIRS}
  ${GENERATED_INCLUDE_DIR}
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(Threads REQUIRED)
target_link_libraries(desktop_sim PRIVATE Threads::Threads)

if(CMAKE_HOST_WIN32)
  target_link_libraries(desktop_sim PRIVATE "ws2_32" -static)
endif()

enable_testing()

set(TEST_FILES
  tests/test_file_flash.cpp
  tests/test_cli_parse.cpp
  tests/test_expressions.cpp
  tests/test_circular_buffer.cpp
  tests/test_gps_format_conversion.cpp
  desktop_hardware_manager.cpp board_config.c
  ${DEVICES_FILES_LIST}
  ${UTILS_FILES_LIST}
  ${SYSTEM_FILES_LIST}
)

set(DESKTOP_SIM_EXE_NAME desktop_sim_test)

add_executable(
  ${DESKTOP_SIM_EXE_NAME}
  ${TEST_FILES}
)

add_dependencies(${DESKTOP_SIM_EXE_NAME} version)

if(CMAKE_HOST_WIN32)
  target_link_libraries(
    ${DESKTOP_SIM_EXE_NAME} PRIVATE
    -static
    gtest_main
  )
else()
  target_link_libraries(
    ${DESKTOP_SIM_EXE_NAME} PRIVATE
    gtest_main
  )
endif()

if(CMAKE_HOST_WIN32)
  target_link_libraries(${DESKTOP_SIM_EXE_NAME} PRIVATE ws2_32 stdc++)
endif()

target_include_directories(${DESKTOP_SIM_EXE_NAME} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${DEVICES_INCLUDE_DIRS}
  ${SYSTEM_INCLUDE_DIRS}
  ${UTILS_INCLUDE_DIRS}
  ${DEVICES_COMMON_INCLUDE_DIRS}
  ${GENERATED_INCLUDE_DIR}
)

include(GoogleTest)
gtest_discover_tests(${DESKTOP_SIM_EXE_NAME})
