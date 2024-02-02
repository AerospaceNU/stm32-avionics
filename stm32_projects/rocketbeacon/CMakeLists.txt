project(rocketbeacon)

file(GLOB_RECURSE gen_files "Drivers/*.c" "Core/*.c" "Core/*.cpp" "USB_DEVICE/*.c" "Middlewares/*.c")

set(STARTUP_FILE Core/Startup/startup_stm32wle5cbux.s)
set_property(SOURCE ${STARTUP_FILE} PROPERTY LANGUAGE C)
set(PRJ_SOURCES
    ${gen_files}
    ${STARTUP_FILE}

    ${UTILS_FILES_LIST}
    ${STM32_DEVICE_DRIVERS_SRCS}
    ${SYSTEM_FILES_LIST}
    ${STM_HARDWARE_MANAGER}
)

set(EXECUTABLE rocketbeacon)

add_executable(${EXECUTABLE} ${PRJ_SOURCES})
add_dependencies(${EXECUTABLE} stm_version)

target_compile_definitions(${EXECUTABLE} PRIVATE
        -DUSE_HAL_DRIVER
        -DSTM32WLE5xx
        -DDEBUG
        -Dtimegm=mktime
)

target_include_directories(${EXECUTABLE} PRIVATE
        Core/Inc
        Drivers/STM32WLxx_HAL_Driver/Inc
        Drivers/CMSIS/Device/ST/STM32WLxx/Include
        Drivers/CMSIS/Include

        ${CMAKE_CURRENT_BINARY_DIR}/../generated/
        
        ${SYSTEM_INCLUDE_DIRS}
        ${UTILS_INCLUDE_DIRS}
        ${DEVICES_COMMON_INCLUDE_DIRS}
        ${STM32_DEVICE_DRIVERS_INCLUDE_DIRS}
)

target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=cortex-m4
        -mthumb
        -mfloat-abi=soft

        -fdata-sections
        -ffunction-sections
        -fno-exceptions
        $<$<COMPILE_LANG_AND_ID:CXX,GNU>:-fno-rtti>
        $<$<COMPILE_LANG_AND_ID:CXX,GNU>:-fno-use-cxa-atexit>
        -fstack-usage

        -Wall
        -MMD
        -MP
        -specs=nano.specs
        -specs=nosys.specs

        -Og
        -g3
        )

target_link_options(${EXECUTABLE} PRIVATE
        -T${CMAKE_CURRENT_SOURCE_DIR}/STM32WLE5CBUX_FLASH.ld
        -mcpu=cortex-m4
        -mthumb
        -mfloat-abi=soft
        -specs=nano.specs
        -lnosys
        -lc
        -lm
        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections
        -Wl,--print-memory-usage
        )

# target_link_libraries(${EXECUTABLE} PRIVATE 
#     ${CMAKE_CURRENT_SOURCE_DIR}/../libs/libarm_cortexM7lfsp_math.a
# )

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-size ${EXECUTABLE}.elf)

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE}.elf ${EXECUTABLE}.hex
        COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE}.elf ${EXECUTABLE}.bin)
