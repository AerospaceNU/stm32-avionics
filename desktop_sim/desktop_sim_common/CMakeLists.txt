execute_process(
  COMMAND(echo "#ifndef UTILS_GIT_INFO_H_" && echo "#define UTILS_GIT_INFO_H_" && echo "#ifdef __cplusplus" && echo "extern \"C\" {" && echo "#endif" && echo "const char* GIT_DESCRIBE = \"`git describe`\";" && echo `git log --pretty=format:"const char* GIT_COMMIT_TIME = \"%ci\";" -n 1` && echo `git log --pretty=format: "const char* GIT_COMMIT_MESSAGE = \"%<(50,trunc)%s\";" -n 1` && echo "#ifdef __cplusplus" && echo "}" && echo "#endif" && echo "#endif  // UTILS_GIT_INFO_H_") > git_info.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set(SIM_COMMON_FILES_LIST
  ${CMAKE_CURRENT_SOURCE_DIR}/desktop_hardware_manager.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/sim_timing.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/nt_interface.cpp
  CACHE INTERNAL "")

set(SIM_COMMON_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
  CACHE INTERNAL "")
