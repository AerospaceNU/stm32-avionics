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
