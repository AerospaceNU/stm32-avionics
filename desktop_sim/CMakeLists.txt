set(SIMULATOR_FILES_LIST
        main.cpp desktop_hardware_manager.cpp board_config.c)


add_executable(desktop_sim ${SIMULATOR_FILES_LIST})

find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(desktop_sim PRIVATE system devices imgui Threads::Threads ${OPENGL_LIBRARIES})
target_include_directories(desktop_sim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ../desktop_ui/src/)

# target_include_directories(desktop_sim PUBLIC ../../external)
