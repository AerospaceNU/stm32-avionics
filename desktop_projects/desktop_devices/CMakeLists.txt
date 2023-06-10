set(DEVICES_FILES_LIST
        ${CMAKE_CURRENT_SOURCE_DIR}/fake_internal_flash.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/flight_replay.cpp 
        ${CMAKE_CURRENT_SOURCE_DIR}/tcp_socket.cpp 
        ${CMAKE_CURRENT_SOURCE_DIR}/print_pyro.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/file_backed_flash.cpp
        CACHE INTERNAL "")

set(DEVICES_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "")
