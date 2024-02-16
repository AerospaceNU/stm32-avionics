cmake_minimum_required(VERSION 3.5.1)
project (client)
option(JS_ONLY "Compiles to native JS (No WASM)" OFF)

SET(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)

file(GLOB SOURCES 
    src/*.cpp 
    ../common/system/expressions/*.cpp
    ../common/system/tasks/*.cpp
    ../common/utils/*.cpp
    ../common/utils/*.c
    ../common/system/tasks/cli/*.cpp
    ../common/system/filtering/*.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/public")
add_executable(wasm_meme ${SOURCES})
target_link_libraries(wasm_meme PUBLIC embind)

target_include_directories(wasm_meme PUBLIC
    include
    ../desktop_projects/desktop_devices
    ../common/system/tasks
    ../common/system/states
    ../common/system/tasks/cli
    ../common/system/expressions
    ../common/system/filtering
    ../common/utils
    ../common/system
    ../common/devices_common
)

set(CMAKE_EXECUTABLE_SUFFIX ".js")
# set_target_properties(wasm_meme PROPERTIES LINK_FLAGS "-s WASM=0 -s EXPORTED_FUNCTIONS='[_main]'")

