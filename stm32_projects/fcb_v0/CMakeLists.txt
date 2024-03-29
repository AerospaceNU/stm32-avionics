project(fcb_v0)

file(GLOB_RECURSE gen_files "Drivers/*.c" "Core/*.c" "Core/*.cpp" "USB_DEVICE/*.c" "Middlewares/*.c")

set_property(SOURCE Core/Startup/startup_stm32h750vbtx.s PROPERTY LANGUAGE C)
set(PRJ_SOURCES
    ${gen_files}
    
    ${SYSTEM_FILES_LIST}
    ${UTILS_FILES_LIST}
    ${STM32_DEVICE_DRIVERS_SRCS}
    ${DEVICES_COMMON_INCLUDE_DIRS}
    ${STM_HARDWARE_MANAGER}
    ${CMSIS_DRIVER_FILES}
    
    Core/Startup/startup_stm32h750vbtx.s
)

set(EXECUTABLE fcb_v0)

add_executable(${EXECUTABLE} ${PRJ_SOURCES})
add_dependencies(${EXECUTABLE} stm_version)

target_compile_definitions(${EXECUTABLE} PRIVATE
        -DUSE_HAL_DRIVER
        -DSTM32H750xx
        -DDEBUG
        -Dtimegm=mktime
        )

target_include_directories(${EXECUTABLE} PRIVATE
        Core/Inc
        Drivers/STM32H7xx_HAL_Driver/Inc
        Drivers/CMSIS/Device/ST/STM32H7xx/Include
        Drivers/CMSIS/Include

        ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/App
        ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/Target
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc

        ${SYSTEM_INCLUDE_DIRS}
        ${DEVICES_COMMON_INCLUDE_DIRS}
        ${UTILS_INCLUDE_DIRS}
        ${STM32_DEVICE_DRIVERS_INCLUDE_DIRS}
        ${CMSIS_DSP_INC}

        ${CMAKE_CURRENT_BINARY_DIR}/../generated/
        )

target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=cortex-m7
        -mthumb
        -mfpu=fpv5-d16
        -mfloat-abi=hard

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
        -flto
        )

target_link_options(${EXECUTABLE} PRIVATE
        -T${CMAKE_CURRENT_SOURCE_DIR}/STM32H750VBTX_FLASH.ld
        -mcpu=cortex-m7
        -mthumb
        -mfpu=fpv5-d16
        -mfloat-abi=hard
        -specs=nano.specs
        -specs=nosys.specs
        -lc
        -lm
        -lnosys
        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections
        -Wl,--print-memory-usage
        -flto
        )

target_link_libraries(${EXECUTABLE} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../libs/libarm_cortexM7lfsp_math.a
)

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-size ${EXECUTABLE}.elf)

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE}.elf ${EXECUTABLE}.hex
        COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE}.elf ${EXECUTABLE}.bin)
