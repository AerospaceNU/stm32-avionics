execute_process(
  COMMAND (echo "#ifndef UTILS_GIT_INFO_H_" && echo "#define UTILS_GIT_INFO_H_" && echo "#ifdef __cplusplus" && echo "extern \"C\" {" && echo "#endif" && echo "const char* GIT_DESCRIBE = \"`git describe`\";" && echo `git log --pretty=format:"const char* GIT_COMMIT_TIME = \"%ci\";" -n 1` && echo `git log --pretty=format:"const char* GIT_COMMIT_MESSAGE = \"%<(50,trunc)%s\";" -n 1` && echo "#ifdef __cplusplus" && echo "}" && echo "#endif" && echo "#endif  // UTILS_GIT_INFO_H_") > git_info.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
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

target_include_directories(desktop_sim PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${DEVICES_INCLUDE_DIRS}
  ${SYSTEM_INCLUDE_DIRS}
  ${UTILS_INCLUDE_DIRS}
  ${DEVICES_COMMON_INCLUDE_DIRS}
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(Threads REQUIRED)
target_link_libraries(desktop_sim PRIVATE Threads::Threads)

enable_testing()

set(TEST_FILES
  tests/test_file_flash.cpp
  tests/test_cli_parse.cpp
  tests/test_expressions.cpp
  desktop_hardware_manager.cpp board_config.c
  ${DEVICES_FILES_LIST}
  ${UTILS_FILES_LIST}
  ${SYSTEM_FILES_LIST}
)

add_executable(
  desktop_sim_test
  ${TEST_FILES}
)

target_link_libraries(
  desktop_sim_test
  gtest_main
)

target_include_directories(desktop_sim_test PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${DEVICES_INCLUDE_DIRS}
  ${SYSTEM_INCLUDE_DIRS}
  ${UTILS_INCLUDE_DIRS}
  ${DEVICES_COMMON_INCLUDE_DIRS}
)

include(GoogleTest)
gtest_discover_tests(desktop_sim_test)
