set(UTILS_FILES_LIST
    circular_buffer.c
    dtoa.c
    small_strtod.c
    string_slice.cpp
    crc16.cpp
)

add_library(utils ${UTILS_FILES_LIST})

target_include_directories(utils PUBLIC .)