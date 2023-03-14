execute_process(
  COMMAND (echo "#ifndef UTILS_GIT_INFO_H_" && echo "#define UTILS_GIT_INFO_H_" && echo "#ifdef __cplusplus" && echo "extern \"C\" {" && echo "#endif" && echo "const char* GIT_DESCRIBE = \"`git describe`\";" && echo `git log --pretty=format:"const char* GIT_COMMIT_TIME = \"%ci\";" -n 1` && echo `git log --pretty=format:"const char* GIT_COMMIT_MESSAGE = \"%<(50,trunc)%s\";" -n 1` && echo "#ifdef __cplusplus" && echo "}" && echo "#endif" && echo "#endif  // UTILS_GIT_INFO_H_") > git_info.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set(SIMULATOR_FILES_LIST
        main.cpp desktop_hardware_manager.cpp board_config.c)

add_executable(desktop_sim ${SIMULATOR_FILES_LIST})

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(desktop_sim PRIVATE system devices)
target_include_directories(desktop_sim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

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
        tests/test_expression_parse.cpp
        desktop_hardware_manager.cpp board_config.c
        )

add_executable(
  desktop_sim_test
  ${TEST_FILES}
)

target_link_libraries(
  desktop_sim_test
  gtest_main
  system
  devices
)
target_include_directories(desktop_sim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

include(GoogleTest)
gtest_discover_tests(desktop_sim_test)
