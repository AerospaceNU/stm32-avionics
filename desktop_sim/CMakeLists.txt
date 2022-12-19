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

find_package(Threads REQUIRED)
target_link_libraries(desktop_sim PRIVATE Threads::Threads)


target_include_directories(desktop_sim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


set(FAT_FILES
        main_test.cpp desktop_hardware_manager.cpp board_config.c)
add_executable(fat_test ${FAT_FILES})
target_link_libraries(fat_test PRIVATE devices)
target_link_libraries(fat_test PRIVATE system)
target_include_directories(fat_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(desktop_sim PRIVATE Threads::Threads)
