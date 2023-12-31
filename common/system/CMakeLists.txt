set(SYSTEM_FILES_LIST
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/cli.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/radio_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/scheduler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/state_log.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/data_log.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/trigger_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/buzzer_heartbeat.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/data_offload.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/cli/task_cli_config.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/cli/task_cli_help.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/cli/task_cli_main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/cli/task_cli_trigger.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/cli/task_cli_sense.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/cli/task_cli_linecutter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/cli/task_cli_version.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/states/flight_state.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/states/states_interface.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/states/state_ascent.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/states/state_descent.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/states/state_initialize.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/states/state_post_flight.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/states/state_pre_flight.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/states/cli/state_cli_erase_flash.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/states/cli/state_cli_offload.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/states/cli/state_cli_temp.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/filtering/altitude_kalman.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/filtering/filters.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/filtering/orientation_estimator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/filtering/filterdata_types.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/expressions/binary_func_expression.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/expressions/expression_store.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/expressions/expression.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/expressions/unary_func_expression.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/expressions/const_expression.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/expressions/empty_expression.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/expressions/event_expression.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/expressions/var_expression.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/expressions/expression_builder.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/event_manager.cpp
        CACHE INTERNAL "")

set(SYSTEM_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/states
        ${CMAKE_CURRENT_SOURCE_DIR}/states/cli
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/cli
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks
        ${CMAKE_CURRENT_SOURCE_DIR}/expressions
        ${CMAKE_CURRENT_SOURCE_DIR}/filtering
        ${CMAKE_CURRENT_SOURCE_DIR}
        CACHE INTERNAL "")

if (PROJECT_BUILD_TYPE MATCHES "stm32")
    message("Adding CMSIS DSP")
    set(CMSIS_DSP_INC ${CMAKE_CURRENT_SOURCE_DIR}/cmsis CACHE INTERNAL "")
    set(STM_HARDWARE_MANAGER ${CMAKE_CURRENT_SOURCE_DIR}/hardware_manager.cpp CACHE INTERNAL "")
endif ()
