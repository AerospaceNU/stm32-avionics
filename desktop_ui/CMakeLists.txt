cmake_minimum_required(VERSION 3.18)
project(desktop_ui)

# Potentially force the binary we make to be static
set(BUILD_SHARED_LIBS OFF)

# Add the allwpilib subdirectory to pull in wpigui, imgui, etc
add_subdirectory(allwpilib)

# Add our main executable.
file(GLOB desktop_gui_src src/*.cpp)
add_executable(desktop_gui ${desktop_gui_src})

# Add target link libraries for executable.
if(WIN32)
    set_target_properties(desktop_gui PROPERTIES LINK_FLAGS "Gdi32.lib Shell32.lib d3d11.lib d3dcompiler.lib")
elseif(APPLE)
    set_target_properties(desktop_gui PROPERTIES LINK_FLAGS "-framework Metal -framework QuartzCore")
elseif(UNIX)
    set_target_properties(desktop_gui PROPERTIES LINK_FLAGS "-lX11")
endif()
target_link_libraries(desktop_gui PRIVATE libglassnt libglass ntcore wpimath wpiutil wpigui imgui)
