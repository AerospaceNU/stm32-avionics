# Grab gtest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

set(TEST_FILES
  test_file_flash.cpp
  # test_flight.cpp
  test_cli_parse.cpp
  test_trigger_manager.cpp
  board_config.c
  sim_hardware_manager.cpp  
)

add_executable(
  desktop_sim_test
  ${TEST_FILES}
  ${SIM_COMMON_FILES_LIST}
  ${DEVICES_FILES_LIST}
  ${UTILS_FILES_LIST}
  ${SYSTEM_FILES_LIST}
)

target_link_libraries(desktop_sim_test gtest_main)
target_include_directories(desktop_sim_test PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${SIM_COMMON_INCLUDE_DIRS}
  ${DEVICES_INCLUDE_DIRS}
  ${SYSTEM_INCLUDE_DIRS}
  ${UTILS_INCLUDE_DIRS}
)

include(GoogleTest)
gtest_discover_tests(desktop_sim_test)