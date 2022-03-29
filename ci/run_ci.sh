#!/bin/bash

# Clang-format
for file in $(find Devices System Utils -type f)
do
	clang-format -i $file
done

# Cpplint
cpplint --counting=detailed --recursive Devices/ System/ Utils/