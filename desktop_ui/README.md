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

hi 3 -> hi 4 takes a while (hangs on including x11 support)

