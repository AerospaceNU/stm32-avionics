STM32 avionics board repo

# Getting Started

This guide should get you through your first build of the project.

## Installing Software and Configuring the Sources

First, pull this git repository using the following command from a directory of your liking.

`git clone https://gitlab.com/aeronu/dollar-per-foot/stm32-avionics`

**Note:** you must have git installed. You can install git bash [here](https://gitforwindows.org/).

Next, open STM32 Cube IDE. If you do not have Cube you can install it from this [link](https://www.st.com/en/development-tools/stm32cubeide.html). Please download version 1.3.0 for compatibility with our code. The default installation options should work.

When opening the IDE, you can use the default workspace. Using the cloned directory may cause problems.

Once the IDE is open, you can import the project. Navigate to `File -> Import -> General -> Existing Projects into Workspace`. Browse for the `H750VBT6-flight-board-v0` folder in `stm32-avionics`. Make sure `Search for nested projects` is checked and that none of the other options are checked. The code should build at this point by clicking on the hammer icon.

## Running Code Using Debug

To run code in debug mode, click on the bug symbol drop down. Click on `H750VBT6-flight-board-v0 Debug`. If it doesn't show up, click on `Debug Configurations...` and find it. The code should load onto the board.

## Correcting Regenerated Sources

The following corrections must be applied any time the source files are rebuilt due to a change in the `.ioc` file.

When the sources are rebuilt, there are a few issues created in the sources. The first problem is because our project is a C++ project, but CubeMX generates only C sources. For this reason, the `main.c` file must be deleted.

There is also an uncorrected bug in the USB driver when interfacing with Windows devices. Open `Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c` and on line 510 include the following line of code:

`memset(pdev->pClassData,0,sizeof(USBD_CDC_HandleTypeDef));`

This correction will allow Windows computers to connect to the board over USB as a Virtual COM device. This issue is not present on Linux computers, however this change should always be made when rebuilding sources for cross-platform compatibility.

## Configuring the Build

Ignore this section if software was installed using the Installing Software section above. This is useful information for creating new projects but unnecessary once the project is already created.

After all of the sources are configured correctly, the build system has to be configured. The source files have to be added to the include paths and the build path.

Begin by navigating to `Project -> Properties` then once that dialog is open, go to `C/C++ Build -> Settings -> Tool Settings -> MCU GCC Compiler` and select the `Include Paths` tab and click on the file icon with a green plus on it. This action should open a pop-up box. Select the `workspace` button in the dialog and include the `Devices` and `System` folders. If any additional directories are created, this process must be repeated.

The `Devices` and `System` directories must also be converted to source folders. In `Project -> Properties`, go to `C/C++ General -> Paths and Symbols` and click on the `Sources` tab.

At this point, the project should be all set to build. Click on the hammer icon and the project should compile.
