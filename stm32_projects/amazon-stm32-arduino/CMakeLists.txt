cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/stm32_gcc.cmake)

project(amazon-stm32-arduino C ASM)

stm32_fetch_cmsis(F4)
stm32_fetch_hal(F4)

find_package(CMSIS COMPONENTS STM32F401CC REQUIRED)
find_package(HAL COMPONENTS STM32F4 RCC GPIO UART PCD LL_USB CORTEX EX_PCD REQUIRED)

set(CORE_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32f4xx_hal_msp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/syscalls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32f4xx_it.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/sysmem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/usart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/App/usb_device.c
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/App/usbd_cdc_if.c
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/App/usbd_desc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/Target/usbd_conf.c
    Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
)

set(CORE_INCS
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/App
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/Target
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
)

set(PROJECT_SOURCES
    ${CORE_SRCS}
)

add_executable(amazon-stm32-arduino ${PROJECT_SOURCES} Core/Inc/stm32f4xx_hal_conf.h)
target_include_directories(amazon-stm32-arduino PUBLIC ${CORE_INCS})
target_link_libraries(amazon-stm32-arduino 
    HAL::STM32::F4::RCC
    HAL::STM32::F4::GPIO
    HAL::STM32::F4::UART
    HAL::STM32::F4::CORTEX
    HAL::STM32::F4::PCD
    HAL::STM32::F4::LL_USB
    HAL::STM32::F4::EX_PCD
    CMSIS::STM32::F401CC 
    STM32::NoSys
)
stm32_print_size_of_target(amazon-stm32-arduino)
