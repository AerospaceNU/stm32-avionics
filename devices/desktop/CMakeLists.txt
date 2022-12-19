set(DEVICES_FILES_LIST
        fake_internal_flash.cpp
        flight_replay.cpp tcp_socket.cpp print_pyro.cpp
        ../minmea.c
        file_backed_flash.cpp)

add_library(devices ${DEVICES_FILES_LIST})

target_include_directories(devices PUBLIC ../../desktop_sim/)
target_include_directories(devices PUBLIC .)
target_include_directories(devices PUBLIC ..)

target_link_libraries(devices PUBLIC utils)
