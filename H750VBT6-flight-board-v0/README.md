# Code Overview
Read this to understand how the code is set up. We will start with important folders with custom code then move to folders with "generated" code.

## Devices
- Files for communicating with hardware connected to the microcontroller
- Calls HAL functions located in Driver folder

## System
- All files that don't deal directly with hardware.
- These files should not call any code from Devices or HAL drivers, except for Hardware Manager.
- Scheduler: Controls all states, including running current state and transitioning between states.
- States: Hold an init, run, and cleanup function for scheduler to call. Running returns an end condition to help the scheduler determine what to do next.
- Hardware Manager: Wrapper for all calls to Devices folder. Allows simplification from system level, such as calling a function to get all sensor data, initialize all sensor data, etc.
- Others, like Data Log, Data Transmission, Filters: Files that don't need to know about hardware directly because the underlying implementation doesn't matter. For instance, it doesn't matter how the flash or radio are set up to call a function from hardware manager to control them.

## Core
- This folder is full of generated code. It will change when you generate code from the configuration GUI.
- The only reason to edit files in this folder directly is to start your system level code in main.cpp. If editing, only write code in between the USER_CODE_BEGIN USER_CODE_END comments.
- We replaced the generated main.c with a main.cpp. Whenever code is generated, you must delete the generated main.c, or building the code will fail.

## Drivers
- HAL drivers
- Directory is created after generating code from the configuration GUI. The contents of each file should never change after a project is created.

## Middlewares
- Contains third-party tools, such as ST USB Device library. Folders may be generated or imported.

## Other Important Files
- ioc: Configuration GUI to generate code for the Core folder. This is where all settings for peripherals should be changed.