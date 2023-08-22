set(STM32_DEVICE_DRIVERS_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/accel_h3lis331dl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/adc_device.c
        ${CMAKE_CURRENT_SOURCE_DIR}/adc_mcp3564.c
        ${CMAKE_CURRENT_SOURCE_DIR}/barometer_ms5607.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ble_chip_nrf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ble_client_std.c
        ${CMAKE_CURRENT_SOURCE_DIR}/buzzer_pwm.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dc_motor_pwm.c
        ${CMAKE_CURRENT_SOURCE_DIR}/flash_mb85rsx.c
        ${CMAKE_CURRENT_SOURCE_DIR}/flash_s25flx.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gps.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hal_callbacks.c
        ${CMAKE_CURRENT_SOURCE_DIR}/imu_icm20600.c
        ${CMAKE_CURRENT_SOURCE_DIR}/imu_iis2mdc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/imu_lsm9ds1.c
        ${CMAKE_CURRENT_SOURCE_DIR}/internal_flash.c
        ${CMAKE_CURRENT_SOURCE_DIR}/led.c
        ${CMAKE_CURRENT_SOURCE_DIR}/line_cutter_ble.c
        ${CMAKE_CURRENT_SOURCE_DIR}/mag_iis2mdc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/minmea.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pyro_digital.c
        ${CMAKE_CURRENT_SOURCE_DIR}/radioconfig
        ${CMAKE_CURRENT_SOURCE_DIR}/servo_pwm.c
        ${CMAKE_CURRENT_SOURCE_DIR}/spi_driver.c
        ${CMAKE_CURRENT_SOURCE_DIR}/temp_max31855.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ti_radio.c
        ${CMAKE_CURRENT_SOURCE_DIR}/usb_std.c
        ${CMAKE_CURRENT_SOURCE_DIR}/vbat_ina226.c
        CACHE INTERNAL "")

set(STM32_DEVICE_DRIVERS_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/radioconfig
        ${CMAKE_CURRENT_SOURCE_DIR}
        CACHE INTERNAL "")
