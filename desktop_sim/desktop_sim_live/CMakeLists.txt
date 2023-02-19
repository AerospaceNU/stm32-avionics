execute_process(
  COMMAND (echo "#ifndef UTILS_GIT_INFO_H_" && echo "#define UTILS_GIT_INFO_H_" && echo "#ifdef __cplusplus" && echo "extern \"C\" {" && echo "#endif" && echo "const char* GIT_DESCRIBE = \"`git describe`\";" && echo `git log --pretty=format:"const char* GIT_COMMIT_TIME = \"%ci\";" -n 1` && echo `git log --pretty=format:"const char* GIT_COMMIT_MESSAGE = \"%<(50,trunc)%s\";" -n 1` && echo "#ifdef __cplusplus" && echo "}" && echo "#endif" && echo "#endif  // UTILS_GIT_INFO_H_") > git_info.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set(SIMULATOR_FILES_LIST
        main.cpp board_config.c board_config.h)

add_executable(desktop_sim_live 
  ${SIMULATOR_FILES_LIST}
  ${SIM_COMMON_FILES_LIST}
  ${DEVICES_FILES_LIST}
  ${UTILS_FILES_LIST}
  ${SYSTEM_FILES_LIST}
)

# INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(desktop_sim_live PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${SIM_COMMON_INCLUDE_DIRS}
  ${DEVICES_INCLUDE_DIRS}
  ${SYSTEM_INCLUDE_DIRS}
  ${UTILS_INCLUDE_DIRS}
  )

find_package(Threads REQUIRED)
target_link_libraries(desktop_sim_live PRIVATE Threads::Threads ntcore wpiutil)
