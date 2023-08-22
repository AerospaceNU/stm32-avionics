cmake_minimum_required(VERSION 3.18)
project(desktop_ui)

set(BUILD_SHARED_LIBS OFF)

message("Hi 1")

INCLUDE(FetchContent)

# Package versions of stuff
if(NOT WPILIB_VERSION)
    set(WPILIB_VERSION 2023.4.3)
endif()
if(NOT WPILIB_PLATFORM)
    set(WPILIB_PLATFORM windowsx86-64)
endif()


function(DOWNLOAD_WPILIB name1 name2)
    message("Downloading ${name1} / ${name2}")
    FetchContent_Declare(
        ${name1}_lib
        URL https://frcmaven.wpi.edu/artifactory/release/edu/wpi/first/${name1}/${name2}/${WPILIB_VERSION}/${name2}-${WPILIB_VERSION}-${WPILIB_PLATFORM}static.zip
    )
    FetchContent_Declare(
        ${name1}_header
        URL https://frcmaven.wpi.edu/artifactory/release/edu/wpi/first/${name1}/${name2}/${WPILIB_VERSION}/${name2}-${WPILIB_VERSION}-headers.zip
    )

    FetchContent_MakeAvailable(
        ${name1}_lib
        ${name1}_header
    )

    # This probably doesn't work great with shared libraries, but I don't care about those right now
    file(GLOB_RECURSE ${name1}_LIB_PATH 
        "${${name1}_lib_SOURCE_DIR}/**/*.lib"
        "${${name1}_lib_SOURCE_DIR}/**/*.a"
    )
    SET(${name1}_LIB_PATH ${${name1}_LIB_PATH} PARENT_SCOPE)
    SET(${name1}_INCLUDE_PATH ${${name1}_header_SOURCE_DIR} PARENT_SCOPE)

    message("${name1} is at: ${${name1}_header_SOURCE_DIR} / ${${name1}_LIB_PATH}")
endfunction()

DOWNLOAD_WPILIB(wpigui wpigui-cpp)
DOWNLOAD_WPILIB(glass libglass)
DOWNLOAD_WPILIB(wpiutil wpiutil-cpp)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY  https://github.com/ocornut/imgui.git
    GIT_TAG         3ea0fad204e994d669f79ed29dcaf61cd5cb571d
)
FetchContent_MakeAvailable(imgui)
file(GLOB imgui_sources ${imgui_SOURCE_DIR}/*.cpp ${imgui_SOURCE_DIR}/misc/cpp/*.cpp)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}
)
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GL3W)
if (MSVC)
    target_sources(imgui PRIVATE ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp)
else()
    if (APPLE)
        target_compile_options(imgui PRIVATE -fobjc-arc)
        set_target_properties(imgui PROPERTIES LINK_FLAGS "-framework Metal -framework QuartzCore")
        target_sources(imgui PRIVATE ${imgui_SOURCE_DIR}/backends/imgui_impl_metal.mm)
    else()
        target_sources(imgui PRIVATE ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
    endif()
endif()

# -- Now we can create our executable and link to dear-imgui --

find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(desktop_gui 
    src/main.cpp 
    src/App.cpp
)
target_link_libraries(desktop_gui PRIVATE 
    ${wpigui_LIB_PATH} 
    ${glass_LIB_PATH}
    ${wpiutil_LIB_PATH}
    imgui
)
target_include_directories(desktop_gui PRIVATE 
    ${wpigui_INCLUDE_PATH}
    ${glass_INCLUDE_PATH}
    ${wpiutil_INCLUDE_PATH}
)
