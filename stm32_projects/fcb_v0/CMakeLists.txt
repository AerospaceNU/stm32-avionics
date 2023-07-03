cmake_minimum_required(VERSION 3.16)

project(fcb_v0 C CXX ASM)

stm32_fetch_cmsis(H7)
stm32_fetch_hal(H7)

find_package(CMSIS COMPONENTS STM32H750VB_M7 REQUIRED)
# find_package(HAL COMPONENTS 
#     STM32H7_M7 RCC DMA RTC I2C GPIO UART PCD LL_USB CORTEX
#     EX_PCD EX_TIM EX_UART SPI TIM ADC IWDG EX_I2C EX_RCC
#     EX_PWR FLASH PWR EX_ADC
# REQUIRED)

set(CORE_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/adc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/board_config.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/debug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/i2c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/iwdg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/rtc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/spi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32h7xx_hal_msp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32h7xx_it.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/syscalls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/sysmem.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/system_stm32h7xx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/usart.c

    ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/App/usb_device.c
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/App/usbd_cdc_if.c
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/App/usbd_desc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/Target/usbd_conf.c
    Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c

    # ${CMAKE_SOURCE_DIR}/build/_deps/stm32-hal-h7-src/Src/stm32h7xx_hal_pwr_ex.c
)

set(CORE_INCS
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/App
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/Target
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
)

file(GLOB CMSIS_DRIVER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/*.c)

set(PROJECT_SOURCES
    ${CORE_SRCS}
    ${SYSTEM_FILES_LIST}
    ${UTILS_FILES_LIST}
    ${STM32_DEVICE_DRIVERS_SRCS}
    ${STM_HARDWARE_MANAGER}
    ${CMSIS_DRIVER_FILES}
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -Og")
add_compile_definitions(
    STM32H7
    STM32H750xx
    timegm=mktime
    USE_HAL_DRIVER
    # DEBUG
)

add_executable(fcb_v0 ${PROJECT_SOURCES} Core/Inc/stm32h7xx_hal_conf.h)
target_include_directories(fcb_v0 PUBLIC 
    ${CORE_INCS} 
    ${SYSTEM_INCLUDE_DIRS}
    ${DEVICES_COMMON_INCLUDE_DIRS}
    ${UTILS_INCLUDE_DIRS}
    ${STM32_DEVICE_DRIVERS_INCLUDE_DIRS}
    ${CMSIS_DSP_INC}
    Drivers/CMSIS/Device/ST/STM32H7xx/Include
    Drivers/CMSIS/Include
    Drivers/STM32H7xx_HAL_Driver/Inc
)
target_compile_options(fcb_v0 PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:-Wno-volatile>")

target_link_libraries(fcb_v0 
    # HAL::STM32::H7::M7::ADC
    # HAL::STM32::H7::M7::CORTEX
    # HAL::STM32::H7::M7::DMA
    # HAL::STM32::H7::M7::FLASH
    # HAL::STM32::H7::M7::GPIO
    # HAL::STM32::H7::M7::I2C
    # HAL::STM32::H7::M7::IWDG
    # HAL::STM32::H7::M7::PCD
    # HAL::STM32::H7::M7::UART
    # HAL::STM32::H7::M7::PWR
    # HAL::STM32::H7::M7::RCC
    # HAL::STM32::H7::M7::RTC
    # HAL::STM32::H7::M7::SPI
    # HAL::STM32::H7::M7::TIM

    # HAL::STM32::H7::M7::EX_ADC
    # HAL::STM32::H7::M7::EX_I2C
    # HAL::STM32::H7::M7::EX_RCC
    # HAL::STM32::H7::M7::EX_PCD
    # HAL::STM32::H7::M7::EX_TIM
    # HAL::STM32::H7::M7::EX_UART

    # HAL::STM32::H7::M7::LL_USB

    # CMSIS::STM32::H7::M7
    CMSIS::STM32::H750VB::M7
    # STM32::Nano

    ${CMAKE_CURRENT_SOURCE_DIR}/../libs/libarm_cortexM7lfsp_math.a
)
stm32_add_linker_script(fcb_v0 PRIVATE STM32H750VBTX_FLASH.ld)
stm32_print_size_of_target(fcb_v0)
