project(amazon-stm32-arduino)

file(GLOB_RECURSE gen_files "Drivers/*.c" "Core/*.c" "Core/*.cpp" "USB_DEVICE/*.c" "Middlewares/*.c")

set_property(SOURCE Core/Startup/startup_stm32f401ccux.s PROPERTY LANGUAGE C)
set(PRJ_SOURCES
    ${gen_files}
    Core/Startup/startup_stm32f401ccux.s
)

set(EXECUTABLE amazon-stm32-arduino)

add_executable(${EXECUTABLE} ${PRJ_SOURCES})
add_dependencies(${EXECUTABLE} stm_version)

target_compile_definitions(${EXECUTABLE} PRIVATE
        -DUSE_HAL_DRIVER
        -DSTM32F401xC
        -DDEBUG
        -Dtimegm=mktime
        )

target_include_directories(${EXECUTABLE} PRIVATE
        Core/Inc
        Drivers/STM32F4xx_HAL_Driver/Inc
        Drivers/CMSIS/Device/ST/STM32F4xx/Include
        Drivers/CMSIS/Include

        ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/App
        ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/Target
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc

        ${CMAKE_CURRENT_BINARY_DIR}/../generated/
        )

target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
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
        )

target_link_options(${EXECUTABLE} PRIVATE
        -T${CMAKE_CURRENT_SOURCE_DIR}/STM32F401CCUX_FLASH.ld
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
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
