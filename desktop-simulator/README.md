# Desktop Simulation

Code above the hardware manager is shared with flight code, while code "below" it is backed by old flight data. Build instructions:


```
mkdir build
cd build 
cmake -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ ..
cmake --build .
<move a flight CSV into the build directory, must be named output-post.csv>
./desktop-simulator/desktop_sim
```
