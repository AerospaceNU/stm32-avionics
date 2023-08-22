# Desktop dear-imgui GUI

Designed to be a stripped down, portable version of our GUI. In addition to the usual cmake/c compiler deps, you'll also need opengl and glfw with
`sudo apt-get install -y libglfw3-dev`

The resulting executable will link to:
```
matt@LAPTOP-F96B9E90:~/stm32-avionics/desktop_ui/build$ ldd desktop_gui 
        linux-vdso.so.1 (0x00007ffd579a9000)
        libOpenGL.so.0 => /lib/x86_64-linux-gnu/libOpenGL.so.0 (0x00007f291a667000)
        libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f291a43b000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f291a354000)
        libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f291a334000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f291a10c000)
        libGLdispatch.so.0 => /lib/x86_64-linux-gnu/libGLdispatch.so.0 (0x00007f291a052000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f291ab84000)
```

We expect:
```
OPENGL_opengl_LIBRARY:FILEPATH=/usr/lib/x86_64-linux-gnu/libOpenGL.so
OPENGL_glx_LIBRARY:FILEPATH=/usr/lib/x86_64-linux-gnu/libGLX.so
OPENGL_INCLUDE_DIR:PATH=/usr/include
```

matt@LAPTOP-F96B9E90:~/stm32-avionics/build_desktop$ ldconfig -p | grep EGL
        libEGL_mesa.so.0 (libc6,x86-64) => /lib/x86_64-linux-gnu/libEGL_mesa.so.0
        libEGL.so.1 (libc6,x86-64) => /lib/x86_64-linux-gnu/libEGL.so.1
        libEGL.so (libc6,x86-64) => /lib/x86_64-linux-gnu/libEGL.so


    CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH: 1
    CMAKE_FIND_USE_CMAKE_SYSTEM_PATH: 1
    CMAKE_FIND_USE_INSTALL_PREFIX: 1

  find_path considered the following locations:

    /home/matt/arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi/bin/GL/gl.h
    /home/matt/.vscode-server/bin/6c3e3dba23e8fadc360aed75ce363ba185c49794/bin/remote-cli/GL/gl.h
    /home/matt/.local/bin/GL/gl.h
    /usr/local/sbin/GL/gl.h
    /usr/local/bin/GL/gl.h
    /usr/sbin/GL/gl.h
    /usr/bin/GL/gl.h
    /sbin/GL/gl.h
    /bin/GL/gl.h
    /usr/games/GL/gl.h
    /usr/local/games/GL/gl.h
    /usr/lib/wsl/lib/GL/gl.h
    /mnt/c/Program Files (x86)/NVIDIA Corporation/PhysX/Common/GL/gl.h
    /mnt/d/Program Files/MATLAB/R2020b/bin/GL/gl.h
    /mnt/c/windows/system32/GL/gl.h
    /mnt/c/windows/GL/gl.h
    /mnt/c/windows/System32/Wbem/GL/gl.h
    /mnt/c/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.6/bin/GL/gl.h
    /mnt/c/windows/System32/WindowsPowerShell/v1.0/GL/gl.h
    /mnt/c/windows/System32/OpenSSH/GL/gl.h
    /mnt/c/WINDOWS/system32/GL/gl.h
    /mnt/c/WINDOWS/GL/gl.h
    /mnt/c/WINDOWS/System32/Wbem/GL/gl.h
    /mnt/c/WINDOWS/System32/WindowsPowerShell/v1.0/GL/gl.h
    /mnt/c/WINDOWS/System32/OpenSSH/GL/gl.h
    /mnt/d/Program Files/Git/cmd/GL/gl.h
    /mnt/c/Users/matth/AppData/Local/Programs/Python/Python38-32/Scripts/GL/gl.h
    /mnt/c/Users/matth/.jdks/liberica-11.0.7/bin/GL/gl.h
    /mnt/c/Users/matth/AppData/Roaming/Python/Python38/Scripts/GL/gl.h
    /mnt/d/Program Files/CMake/bin/GL/gl.h
    /mnt/d/Program Files/nodejs/GL/gl.h
    /mnt/c/Program Files/Microsoft SQL Server/Client SDK/ODBC/110/Tools/Binn/GL/gl.h
    /mnt/c/Program Files (x86)/Microsoft SQL Server/120/Tools/Binn/GL/gl.h
    /mnt/c/Program Files/Microsoft SQL Server/120/Tools/Binn/GL/gl.h
    /mnt/c/Program Files/Microsoft SQL Server/120/DTS/Binn/GL/gl.h
    /mnt/c/Program Files (x86)/Windows Kits/8.1/Windows Performance Toolkit/GL/gl.h
    /mnt/c/Users/matth/AppData/Roaming/npm/GL/gl.h
    /mnt/c/Program Files/dotnet/GL/gl.h
    /mnt/c/Program Files (x86)/IVI Foundation/VISA/WinNT/Bin/GL/gl.h
    /mnt/c/Program Files/IVI Foundation/VISA/Win64/Bin/GL/gl.h
    /mnt/d/Documents/Android/SDK/platform-tools/GL/gl.h
    /mnt/c/Program Files/NVIDIA Corporation/NVIDIA NvDLISR/GL/gl.h
    /mnt/c/Users/matth/.platformio/penv/Scripts/GL/gl.h
    /mnt/c/Users/matth/AppData/Local/Programs/Python/Python38-32/GL/gl.h
    /mnt/c/Users/matth/AppData/Local/Microsoft/WindowsApps/GL/gl.h
    /mnt/c/Users/matth/AppData/Local/GitHubDesktop/bin/GL/gl.h
    /mnt/c/Users/matth/AppData/Local/Programs/Microsoft VS Code/bin/GL/gl.h
    /mnt/c/Program Files (x86)/GitHub CLI/GL/gl.h
    /mnt/c/Program Files (x86)/STMicroelectronics/STM32 ST-LINK Utility/ST-LINK Utility/GL/gl.h
    /mnt/c/Program Files (x86)/Arduino/GL/gl.h
    /mnt/c/Program Files/PuTTY/GL/gl.h
    /mnt/d/Program Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/GL/gl.h
    /mnt/c/Program Files/NVIDIA Corporation/Nsight Compute 2022.1.1/GL/gl.h
    /mnt/c/Program Files/LLVM/bin/GL/gl.h
    /mnt/c/ProgramData/chocolatey/bin/GL/gl.h
    /mnt/c/Program Files/CMake/bin/GL/gl.h
    /mnt/c/Program Files/Docker/Docker/resources/bin/GL/gl.h
    /mnt/c/ProgramData/DockerDesktop/version-bin/GL/gl.h
    /mnt/c/Program Files/Microsoft SQL Server/Client SDK/ODBC/170/Tools/Binn/GL/gl.h
    /mnt/c/Program Files (x86)/Microsoft SQL Server/150/Tools/Binn/GL/gl.h
    /mnt/c/Program Files/Microsoft SQL Server/150/Tools/Binn/GL/gl.h
    /mnt/c/Program Files/Microsoft SQL Server/150/DTS/Binn/GL/gl.h
    /mnt/c/Program Files/usbipd-win/GL/gl.h
    /mnt/d/ST/STM32CubeIDE_1.4.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.200.202301161003/tools/bin/GL/gl.h
    /mnt/c/Users/matth/AppData/Local/Pandoc/GL/gl.h
    /mnt/d/msys64/mingw64/bin/GL/gl.h
    /snap/bin/GL/gl.h
    /include/GL/gl.h
    /usr/share/doc/NVIDIA_GLX-1.0/include/GL/gl.h
    /usr/openwin/share/include/GL/gl.h
    /opt/graphics/OpenGL/include/GL/gl.h

  The item was not found.

Call Stack (most recent call first):
  desktop_ui/CMakeLists.txt:124 (find_package)


. /usr/include/GL/gl.h