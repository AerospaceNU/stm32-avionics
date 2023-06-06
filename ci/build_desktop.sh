#!/bin/bash

touch CMakeListsPrivate.txt

(echo "#ifndef UTILS_GIT_INFO_H_" && echo "#define UTILS_GIT_INFO_H_" && echo "#ifdef __cplusplus" && echo "extern \"C\" {" && echo "#endif" && echo "const char* GIT_DESCRIBE = \"`git describe`\";" && echo `git log --pretty=format:"const char* GIT_COMMIT_TIME = \"%ci\";" -n 1` && echo `git log --pretty=format:"const char* GIT_COMMIT_MESSAGE = \"%<(50,trunc)%s\";" -n 1` && echo "#ifdef __cplusplus" && echo "}" && echo "#endif" && echo "#endif  // UTILS_GIT_INFO_H_") > ./common/utils/git_info.h
pushd .
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=desktop

cmake --build . --target desktop_sim desktop_sim_test -j
if [ $? -ne 0 ];
then
    exit -1
fi

popd
./build/desktop_projects/desktop_sim/desktop_sim_test
if [ $? -ne 0 ];
then
    exit -1
fi
