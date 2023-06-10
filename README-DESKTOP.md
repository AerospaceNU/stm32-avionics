# Building for desktop

STM32-avionics has support for building unit tests and local simulation for Windows, Mac and Linux. Unit tests are run using google test, and test system-level code. The local simulation communicates with the Groundstation GUI by simulating radio telemetry messages using log data from past flights.

## Building for Linux/Mac

Install CMake, gcc and g++ using your favorite package manager, then configure the project:

```
cmake -B build -DCMAKE_BUILD_TYPE=desktop
```

## Building for Windows using WSL

Install Ubuntu on your computer using WSL, and then build normally using the Linux instructions above. WSL 2 users may need to change the Groundstation GUI to point to WSL's IP instead of localhost.

## Building for Windows (Native)

Windows uses MSYS2 to provide most of the same linux-looking libraries. 

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
