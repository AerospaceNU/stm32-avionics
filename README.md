STM32 avionics board repo

# Getting Started

This guide should get you through your first build of the project for stm32. For local simulation, read the [desktop readme](README-DESKTOP.md).

## Installing Software and Configuring the Sources

First, pull this git repository using the following command from a directory of your liking.

`git clone https://gitlab.com/aeronu/dollar-per-foot/stm32-avionics`

**Note:** you must have git installed. You can install git for windows [here](https://gitforwindows.org/).

Next, open STM32 Cube IDE. If you do not have Cube you can install it from this [link](https://www.st.com/en/development-tools/stm32cubeide.html). Please download version 1.12.0 for compatibility with our code.

When opening the IDE, you can use the default workspace. Using the cloned directory may cause problems.

Once the IDE is open, you can import the project. Navigate to `File -> Import -> General -> Existing Projects into Workspace`. Browse for the `stm32-avionics` folder. Make sure `Search for nested projects` is checked and that none of the other options are checked, then import all projects. All projects should build at this point by clicking on the hammer icon when one of their files is open.

## Installing Formatting & Linting

To keep commonality among code, we will be using both a formatter and a linter for C/C++. The formatter is called clang-format, and the linter is called Cpplint.

### clang-format

1. Ensure python is installed on your system. If not, download from the [Python website](https://www.python.org/downloads/). The newest version is preferred. Also ensure Python executable is added to your PATH
2. Run `pip install clang-format`
3. Find clang-format.exe. This should be in your python installation folder in Lib/site-packages/clang_format/data/bin. Record the full path to the clang-format.exe file.

### Cpplint

1. Run `pip install cpplint`
2. Cpplint needs to be an executable to be run in Eclipse as a plugin. Therefore, you need to make an executable. Install pyinstaller with `pip install pyinstaller`, then run `pyinstaller path_to_cpplint.py`.
3. In the folder where you ran pyinstaller, you should see a folder called `dist/cpplint`. Move this folder somewhere where you'll remember, outside the repository. Note there should be a `cpplint.exe` file. This will be used later.

### CppStyle STM32CubeIDE/Eclipse Plugin
1. In IDE, click *Help->Install New Software*. Click Add.
2. For name, type *CppStyle*, and for URL, type *http://www.zhanwei.wang/CppStyle/update*. Follow instructions until end and restart IDE.
3. Go to *Window->Preferences->C/C++->Code Style->Formatter->Code Formatter*. Change to *CppStyle (clang-format)*
4. Go to *Window->Preferences->C/C++->CppStyle*. Change *Clang-format Path* to correct executable recorded in earlier section. Change *Cpplint path* to `cpplint.exe` from previous section.
5. Check *Run clang-format on file save* and *Enable cpplint*.
6. Go to *Preferences->C/C++->Code Analysis*. Check all boxes except:
   - Coding Style/Avoid magic numbers
   - Coding Style/C-Style cast instead of C++ cast
   - Coding Style/Goto statement used
   - Coding Style/Lack of copyright information
   - Coding Style/Line Comments
   - Coding Style/Multiple variable declaration
   - Coding Style/Name convention for function
   - Coding Style/Return with parenthesis
   - Cpplint Issues/Legal
   - Potential Programming Problems/Miss copy constructor or assignment operator
7. By default CppStyle doesn't run on .c files. Staying on *Code Analysis* tab, highlight all items under *Cpplint Issues* by clicking on "Build/c++11", holding shift, and clicking on "Whitespace/todo." Click the *Customize Selected* button underneath. Go to the *Scope* tab and click *Add...* next to *Inclusion patterns.* Type `*.c` and click OK. Apply all changes.
8. When you save a file, both clang-format and cpplint should now run.

## Running Code Using Debug

To run code in debug mode, click on the bug symbol drop down. Click on `fcb_v0` or the equivalent for the specific board version. If it doesn't show up, click on `Debug Configurations...` and find it. The code should load onto the board.

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

## Utils
- Files that could be used anywhere in System or Devices
- These files consist of general helpers and math operations

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
- ioc: Defines configuration GUI to generate code for the Core folder. This is where all settings for peripherals should be changed.
- cproject: Configuration file for all code-related settings. This consists of includes, source folders, compiler options, build configurations, and more.

# Additional Helpful Info

## Correcting Regenerated Sources

The following corrections must be applied any time the source files are rebuilt due to a change in the `.ioc` file.

When the sources are rebuilt, there is one issue created in the sources. Our project is a C++ project, but CubeMX generates only C sources. For this reason, the `main.c` file must be deleted.

## Configuring the Build

Ignore this section if software was installed using the Installing Software section above. This is useful information for creating new projects but unnecessary once the project is already created.

After all of the sources are configured correctly, the build system has to be configured. The source files have to be added to the include paths and the build path.

Begin by navigating to `Project -> Properties` then once that dialog is open, go to `C/C++ Build -> Settings -> Tool Settings -> MCU GCC Compiler` and select the `Include Paths` tab and click on the file icon with a green plus on it. This action should open a pop-up box. Select the `workspace` button in the dialog and include the `Devices` and `System` folders. If any additional directories are created, this process must be repeated.

The `Devices` and `System` directories must also be converted to source folders. In `Project -> Properties`, go to `C/C++ General -> Paths and Symbols` and click on the `Sources` tab.

At this point, the project should be all set to build. Click on the hammer icon and the project should compile.
