add_library(python_interop_example SHARED 
    example.cpp
    ${UTILS_FILES_LIST}
    )

target_include_directories(python_interop_example PRIVATE 
  ${UTILS_INCLUDE_DIRS}
)

add_executable(fec_test
    example.cpp
    ${UTILS_FILES_LIST}
    )

target_include_directories(fec_test PRIVATE 
  ${UTILS_INCLUDE_DIRS}
)