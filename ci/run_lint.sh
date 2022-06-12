#!/bin/bash

# Clang-format
for file in $(find Devices System Utils -type f -iregex '.*\.\(h\|c\|cpp\)')
do
	clang-format --dry-run --Werror $file
        if [ $? -ne 0 ];
        then
            exit -1
        fi
done

# Cpplint
cpplint --counting=detailed --recursive Devices/ System/ Utils/
