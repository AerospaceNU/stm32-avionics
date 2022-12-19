set(SIMULATOR_FILES_LIST
        main.cpp desktop_hardware_manager.cpp board_config.c)

add_executable(desktop_sim ${SIMULATOR_FILES_LIST})


INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(desktop_sim PRIVATE system)
target_link_libraries(desktop_sim PRIVATE devices)
target_include_directories(desktop_sim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Threads REQUIRED)
target_link_libraries(desktop_sim PRIVATE Threads::Threads)

# target_include_directories(desktop_sim PUBLIC ../../external)

set(FAT_FILES
        main_test.cpp desktop_hardware_manager.cpp board_config.c)
add_executable(fat_test ${FAT_FILES})
target_link_libraries(fat_test PRIVATE system)
target_link_libraries(fat_test PRIVATE devices)
target_include_directories(fat_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(desktop_sim PRIVATE Threads::Threads)