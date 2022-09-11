set(SYSTEM_FILES_LIST
        cli.c
        radio_manager.c
        scheduler.cpp
        state_log.c
        data_log.c
        pyro_manager.c

        tasks/buzzer_heartbeat.c
        tasks/data_offload.c
        tasks/cli/task_cli_config.cpp
        tasks/cli/task_cli_help.cpp
        tasks/cli/task_cli_main.cpp
        tasks/cli/task_cli_pyro.cpp
        tasks/cli/task_cli_sense.cpp
        tasks/cli/task_cli_linecutter.cpp
        tasks/cli/task_cli_version.cpp

        states/flight_state.cpp
        states/states_interface.cpp
        states/state_ascent.cpp
        states/state_descent.cpp
        states/state_initialize.cpp
        states/state_post_flight.cpp
        states/state_pre_flight.cpp

        states/cli/state_cli_calibrate.cpp
        states/cli/state_cli_erase_flash.cpp
        states/cli/state_cli_offload.cpp
        states/cli/state_cli_temp.cpp

        filtering/altitude_kalman.cpp
        filtering/filters.cpp
        filtering/orientation_estimator.cpp
        )

add_library(system ${SYSTEM_FILES_LIST})

target_include_directories(system PRIVATE ../desktop_sim/)
target_include_directories(system PUBLIC filtering)
target_include_directories(system PUBLIC states)
target_include_directories(system PUBLIC states/cli)
target_include_directories(system PUBLIC tasks)
target_include_directories(system PUBLIC tasks/cli)
target_include_directories(utils PUBLIC .)

target_link_libraries(system PUBLIC utils)
target_link_libraries(system PUBLIC devices)
