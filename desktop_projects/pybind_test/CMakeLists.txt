add_library(stm32-avionics-py SHARED 
    example.cpp
    ${UTILS_FILES_LIST}
    )

target_include_directories(stm32-avionics-py PRIVATE 
  ${UTILS_INCLUDE_DIRS}
)

add_executable(fec_test
    example.cpp
    ${UTILS_FILES_LIST}
    )

target_include_directories(fec_test PRIVATE 
  ${UTILS_INCLUDE_DIRS}
)