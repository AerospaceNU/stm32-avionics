set(STM32_DEVICE_DRIVERS_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/accel_h3lis331dl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/adc_device.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/adc_mcp3564.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/barometer_ms5607.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ble_chip_nrf.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ble_client_std.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/buzzer_pwm.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dc_motor_pwm.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/flash_mb85rsx.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/flash_s25flx.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gps.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/hal_callbacks.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imu_icm20600.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imu_iis2mdc.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imu_lsm9ds1.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/internal_flash.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/led.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/line_cutter_ble.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mag_iis2mdc.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/minmea.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pyro_digital.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/servo_pwm.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/spi_driver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/temp_max31855.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ti_radio.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/usb_std.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vbat_ina226.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imu/imu_icm42688.cpp
        CACHE INTERNAL "")

set(STM32_DEVICE_DRIVERS_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/radioconfig
        ${CMAKE_CURRENT_SOURCE_DIR}
        CACHE INTERNAL "")
