#!/bin/bash

# Clang-format
for file in $(find common desktop_projects stm32_projects/device_drivers platformio_projects/src platformio_projects/lib/device_drivers platformio_projects/lib/new_hal -type f -iregex '.*\.\(h\|c\|cpp\)')
do
	clang-format-11 -i $file
        if [ $? -ne 0 ];
        then
            exit -1
        fi
done

# Cpplint
cpplint --counting=detailed --filter="-build/c++11" --recursive common/ desktop_projects/ stm32_projects/device_drivers/ platformio_projects/src/ platformio_projects/lib/device_drivers/ platformio_projects/lib/new_hal/
