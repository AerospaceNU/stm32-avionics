set(SIMULATOR_FILES_LIST
        main.cpp desktop_hardware_manager.cpp board_config.c)

add_executable(desktop_sim ${SIMULATOR_FILES_LIST})

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(desktop_sim PRIVATE system)
target_link_libraries(desktop_sim PRIVATE devices)
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

# target_include_directories(desktop_sim PUBLIC ../../external)

enable_testing()

set(TEST_FILES
        tests/test_file_flash.cpp)
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

include(GoogleTest)
gtest_discover_tests(desktop_sim_test)
