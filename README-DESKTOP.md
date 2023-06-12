# Building for desktop

STM32-avionics has support for building unit tests and local simulation for Windows, Mac and Linux. Unit tests are run using google test, and test system-level code. The local simulation communicates with the Groundstation GUI by simulating radio telemetry messages using log data from past flights. WSL should handle forwarding ports between Linux and Windows, so launching desktop sim in Linux and enabling the local simulation interface in the groundstaton should Just Work:tm:.


## Building for Linux/Mac

Install CMake, gcc and g++ using your favorite package manager, then configure the project:

```
cmake -B build -DCMAKE_BUILD_TYPE=desktop
```


## Building for Windows using WSL

Install Ubuntu on your computer using WSL, and then build normally using the Linux instructions above. WSL 2 users may need to change the Groundstation GUI to point to WSL's IP instead of localhost.

I'm going to be following [this guide](https://learn.microsoft.com/en-us/windows/wsl/install):

```
# Required to install WSL and Ubuntu. Exact distribution doesn't matter
wsl --install
wsl --install -d Ubuntu-22.04
# Enter a username and password when prompted, and reboot if prompted after install is finished

# Optional, only if you want to run GUI applications from Linux
wsl --set-default-version 2 
wsl -l -v                    # should report 2
```

After rebooting, run `wsl` from the start menu or from a command prompt to launch WSL. Now let's clone and setup the repo. Create your SSH key with `ssh-keygen` accepting all defaults, then copy the output of `cat ~/.ssh/id_rsa.pub` to [your gitlab ssh keys](https://gitlab.com/-/profile/keys).

```
sudo apt install gcc g++ cmake make

git clone git@gitlab.com:aeronu/dollar-per-foot/stm32-avionics.git

cd stm32-avionics
./ci/build_desktop
```

If the build succeeds, then you've successfully got the code to build! You can use VSCode to edit code by running `code ~/stm32-avionics`, and accepting all prompts to install extensions and use the actual Windows vscode instead of rendering in Linux. 


## Building for Windows (Native)

Windows uses MSYS2 to provide most of the same linux-looking libraries. Note that only gtest tests will actually build, the socket code doesn't work right now

- Install msys2
- Follow https://code.visualstudio.com/docs/cpp/config-mingw
- In the msys2 terminal, run `pacman -S --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja` on step 5 of that guide
- Make sure that when you run cmake, it points to the msys install of cmake!
- Make sure to add msys2 to the path
- Try a full msys upgrade with pacman -Suy if you have weird issues

After doing the above, run the following in Powershell

```
# configure
$gxx_loc = (where.exe g++).Replace("\","/")
$gcc_loc = (where.exe gcc).Replace("\","/")
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=desktop -Wno-dev -DCMAKE_CXX_COMPILER="$gxx_loc" -DCMAKE_C_COMPILER="$gcc_loc"

# for gtest
cmake --build build --target desktop_sim_test
# for local sim
cmake --build build --target desktop_sim
```
