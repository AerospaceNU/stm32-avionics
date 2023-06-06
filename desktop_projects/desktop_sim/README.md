# Desktop Simulation

Code above the hardware manager is shared with flight code, while code "below" it is backed by old flight data. 

## Windows - WSL

WSL is Windows Subsystem for Linux. Essentially, it allows you to have a Linux machine within your Windows OS.
Since the desktop sim code is built in Linux, a Linux distribution is necessary to build the desktop sim.
It is recommended to install Ubuntu as your WSL distribution. WSL itself should come on Windows already, so to install Ubuntu, run

```
wsl --install -d Ubuntu
```

A computer restart may be necessary to complete installation.

## Build And Run Instructions:

To build the code, run the build script `./desktop-simulator/build.sh`

To run the code,
1. Move a flight CSV into the build directory named output-post.csv
2. Run `./build/desktop-simulator/desktop_sim` (or `wsl ./build/desktop-simulator/desktop_sim` if on Windows)
