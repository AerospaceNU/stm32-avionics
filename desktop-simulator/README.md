# Desktop Simulation

Code above the hardware manager is shared with flight code, while code "below" it is backed by old flight data. 

## Build instructions:


```
mkdir build
cd build 
cmake -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ ..
cmake --build .
```

## Running tests

Build like above, then run the following
```
cd build
ctest --test-dir desktop-simulator/
```
