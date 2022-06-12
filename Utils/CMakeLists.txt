set(UTILS_FILES_LIST
    circular_buffer.c
        )

add_library(utils ${UTILS_FILES_LIST})

target_include_directories(utils PUBLIC .)