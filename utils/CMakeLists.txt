set(UTILS_FILES_LIST
    circular_buffer.c
    dtoa.c
    small_strtod.c
)

add_library(utils ${UTILS_FILES_LIST})

target_include_directories(utils PUBLIC .)