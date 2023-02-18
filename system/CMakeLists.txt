set(SYSTEM_FILES_LIST
        ${CMAKE_CURRENT_SOURCE_DIR}/cli.c
        ${CMAKE_CURRENT_SOURCE_DIR}/radio_manager.c
        ${CMAKE_CURRENT_SOURCE_DIR}/scheduler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/state_log.c
        ${CMAKE_CURRENT_SOURCE_DIR}/data_log.c
        ${CMAKE_CURRENT_SOURCE_DIR}/trigger_manager.c
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/buzzer_heartbeat.c
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/data_offload.c
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
        CACHE INTERNAL "")

set(SYSTEM_INCLUDE_DIRS filtering
        ${CMAKE_CURRENT_SOURCE_DIR}/states
        ${CMAKE_CURRENT_SOURCE_DIR}/states/cli
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks/cli
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks
        ${CMAKE_CURRENT_SOURCE_DIR}/filtering
        ${CMAKE_CURRENT_SOURCE_DIR}
        CACHE INTERNAL "")
