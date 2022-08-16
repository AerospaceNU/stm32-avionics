set(SYSTEM_FILES_LIST
        cli.c
        radio_manager.c
        scheduler.cpp
        state_log.c
        data_log.c
        pyro_manager.c

        Tasks/buzzer_heartbeat.cpp
        Tasks/data_offload.c
        Tasks/cli/task_cli_config.cpp
        Tasks/cli/task_cli_help.cpp
        Tasks/cli/task_cli_main.cpp
        Tasks/cli/task_cli_pyro.cpp
        Tasks/cli/task_cli_sense.cpp
        Tasks/cli/task_cli_linecutter.cpp
        Tasks/cli/task_cli_version.cpp

        States/flight_state.cpp
        States/states_interface.cpp
        States/state_ascent.cpp
        States/state_descent.cpp
        States/state_initialize.cpp
        States/state_post_flight.cpp
        States/state_pre_flight.cpp

        States/Cli/state_cli_calibrate.cpp
        States/Cli/state_cli_erase_flash.cpp
        States/Cli/state_cli_offload.cpp
        States/Cli/state_cli_temp.cpp

        Filtering/altitude_kalman.cpp
        Filtering/filters.cpp
        Filtering/orientation_estimator.cpp
        )

add_library(system ${SYSTEM_FILES_LIST})

target_include_directories(system PRIVATE ../desktop-simulator/)
target_include_directories(system PUBLIC Filtering)
target_include_directories(system PUBLIC States)
target_include_directories(system PUBLIC States/Cli)
target_include_directories(system PUBLIC Tasks)
target_include_directories(system PUBLIC Tasks/cli)
target_include_directories(utils PUBLIC .)

target_link_libraries(system PUBLIC utils)
target_link_libraries(system PUBLIC devices)
