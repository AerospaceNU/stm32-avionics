#!/bin/bash
echo -e "#ifndef _GIT_INFO_\n#define _GIT_INFO_\n#ifdef __cplusplus\nextern \"C\" {\n#endif" > ./Utils/git_info.h && echo -e "const char* GIT_DESCRIBE = \"`git describe`\";" >> ./Utils/git_info.h && echo -e `git log --pretty=format:"const char* GIT_COMMIT_TIME =\"%ci\";\nconst char* GIT_COMMIT_MESSAGE =\"%<(50,trunc)%s\";" -n 1` >> ./Utils/git_info.h && echo -e "#ifdef __cplusplus\n}\n#endif\n#endif" >> ./Utils/git_info.h

mkdir build
cd build
cmake ..

cmake --build .
if [ $? -ne 0 ];
then
    exit -1
fi
