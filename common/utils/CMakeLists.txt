set(UTILS_FILES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/bit_helper.c
    ${CMAKE_CURRENT_SOURCE_DIR}/circular_buffer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/crc16.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dtoa.c
    ${CMAKE_CURRENT_SOURCE_DIR}/small_strtod.c
    ${CMAKE_CURRENT_SOURCE_DIR}/string_slice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/string_slice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ti_fec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ti_fec_encode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ti_fec_decode.cpp
    CACHE INTERNAL "")

set(UTILS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "")
