STM32 avionics board repo

# Getting Started

This guide should get you through your first build of the project.

## Installing Software and Configuring the Sources

First, pull this git repository using the following command from a directory of your liking.

`git clone https://gitlab.com/aeronu/dollar-per-foot/stm32-avionics`

**Note:** you must have git installed. You can install git bash [here](https://gitforwindows.org/).

Next, open STM32 Cube IDE. If you do not have Cube you can install it from this [link](https://www.st.com/en/development-tools/stm32cubeide.html). The default installation options should work. 

When opening the IDE set your workspace to the `H750VBT6-flight-board-v0` directory.

Once the workspace is open, create a new project from an .ioc file. To do this navigate to `File -> New -> STM32 Project From STM32CubeMX .ioc File`. This should open an `.ioc` file. Saving this file will regenerate sources.

## Correcting Regenerated Sources

The following corrections must be applied any time the source files are rebuilt due to a change in the `.ioc` file.

When the sources are rebuilt, there are a few issues created in the sources. The first problem is because our project is a C++ project, but CubeMX generates only C sources. For this reason, the `main.c` file must be deleted.

There is also an uncorrected bug in the USB driver when interfacing with Windows devices. Open `Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c` and on line 510 include the following line of code:

`memset(pdev->pClassData,0,sizeof(USBD_CDC_HandleTypeDef));`

This correction will allow Windows computers to connect to the board over USB as a Virtual COM device. This issue is not present on Linux computers, however this change should always be made when rebuilding sources for cross-platform compatibility.

## Configuring the Build

After all of the sources are configured correctly, the build system has to be configured. The source files have to be added to the include paths and the build path.

Begin by navigating to `Project -> Properties` then once that dialog is open, go to `C/C++ Build -> Settings -> Tool Settings -> MCU GCC Compiler` and select the `Include Paths` tab and click on the file icon with a green plus on it. This action should open a pop-up box. Select the `workspace` button in the dialog and include the `Devices` and `System` folders. If any additional directories are created, this process must be repeated.

The `Devices` and `System` directories must also be converted to source folders. In `Project -> Properties`, go to `C/C++ General -> Paths and Symbols` and click on the `Sources` tab.

At this point, the project should be all set to build. Click on the hammer icon and the project should compile.
